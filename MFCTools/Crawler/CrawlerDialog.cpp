
// CrawlerDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Crawler.h"
#include "CrawlerDialog.h"
#include "afxdialogex.h"
#include "Worker.h"
#include "Hook.h"
#include "ProcessDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IDC_SYSTEM_TRAY_ICON		100									//����ͼ��

#define IDM_NULL_COMMAND (WM_USER + 1000)
#define IDM_SHOW (WM_USER + 1001)
#define IDM_CLOSE (WM_USER + 1002)

#define IDI_TIMEER_DRAG				1

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CSkinDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CSkinDialog(CAboutDlg::IDD)
{

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CSkinDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CSkinDialog)
END_MESSAGE_MAP()


// CCrawlerDialog �Ի���
CCrawlerDialog::CCrawlerDialog(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CCrawlerDialog::IDD, pParent)
{
	TCHAR szPath[_CC_MAX_PATH_];
	m_bTipClose = FALSE;
	m_bInitialized = FALSE;
	m_cbStatus = _STATUS_STOPED_;
	m_hFromWnd = NULL;
	m_dwProcessID = 0;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	_cc_get_module_directory(NULL, szPath, _cc_countof(szPath));

	_sntprintf_s(m_szPathFile,_cc_countof(m_szPathFile), _T("%s\\Crowler.cfg"),szPath);
	ReadDBConfigFile(szPath);
}

CCrawlerDialog::~CCrawlerDialog()
{
	SaveConfigFile();
}

void CCrawlerDialog::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_HINT, m_StaticHint);
	DDX_Control(pDX, IDC_CRAWLER_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_BUTTON_SETTING, m_btSetting);
	DDX_Control(pDX, IDC_BUTTON_RUNNING, m_btRuning);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	
}

BEGIN_MESSAGE_MAP(CCrawlerDialog, CSkinDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()

	//�˵���Ϣ
	ON_COMMAND(IDM_SHOW,OnEventShow)
	ON_COMMAND(IDM_CLOSE,OnEventClose)
	ON_COMMAND(IDM_ABOUTBOX,OnEventAboutBox)

	ON_MESSAGE(WM_TRAY_ICON, OnMessageTrayIcon)
	ON_MESSAGE(WM_MESSAGE_EVENT, OnMessageEvent)
	//ON_STN_CLICKED(IDC_PIC_DRAG, OnStnClickedPicDrag)
	ON_BN_CLICKED(IDC_BUTTON_SETTING, &CCrawlerDialog::OnBnClickedSetting)
	ON_BN_CLICKED(IDC_BUTTON_RUNNING, &CCrawlerDialog::OnBnClickedButtonRunning)
	ON_NOTIFY(NM_DBLCLK, IDC_CRAWLER_LIST, OnNMDBLClickList)
END_MESSAGE_MAP()


// CCrawlerDialog ��Ϣ�������

BOOL CCrawlerDialog::OnInitDialog()
{
	__super::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	SetWindowText(gProductName);

	m_StaticHint.SetWindowText(_T("���ڶ�ȡ����"));
	gCrawlerScreenshot.cbSuccess = ReadConfigFile();
	
	CenterWindow();
	ShowWindow(SW_SHOWNOACTIVATE);

	m_StaticDrag.SubclassDlgItem(IDC_PIC_DRAG,this);
	m_StaticAttach.SubclassDlgItem(IDC_PIC_ATTACH,this);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	// Set the ListCtrl style
	m_ListCtrl.InsertColumn(GUI_PROCESS_MODULE_LIST_ICON_ID, GUI_PROCESS_MODULE_LIST_ICON,
		LVCFMT_LEFT,
		GUI_PROCESS_MODULE_LIST_ICON_SIZE
		);
	m_ListCtrl.InsertColumn(GUI_PROCESS_MODULE_LIST_NAME_ID, GUI_PROCESS_MODULE_LIST_NAME,
		LVCFMT_LEFT,
		170
		);

	m_ListCtrl.InsertColumn(GUI_PROCESS_MODULE_LIST_ADDRESS_ID, GUI_PROCESS_MODULE_LIST_ADDRESS,
		LVCFMT_LEFT,
		GUI_PROCESS_MODULE_LIST_ADDRESS_SIZE
		);

	m_ListCtrl.InsertColumn(GUI_PROCESS_MODULE_LIST_BASESIZE_ID, GUI_PROCESS_MODULE_LIST_BASESIZE,
		LVCFMT_LEFT,
		GUI_PROCESS_MODULE_LIST_BASESIZE_SIZE
		);

	//�����и�
	m_ListCtrl.SetItemHeight(22);
	m_ListCtrl.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	//��������
	m_SystemTrayIcon.InitTrayIcon(m_hWnd, IDC_SYSTEM_TRAY_ICON);
	m_SystemTrayIcon.ShowTrayIcon(m_hIcon, gProductName);

	if (!gCrawlerScreenshot.cbSuccess) {
		/*
		INT nWidth = 0;
		for (INT i = 0; i < MAX_CRAWLER_DATA_BOX; i++) {
			gCrawlerScreenshot.nBoxWidth[i] = 100;
			nWidth += 100;
		}
		SetRect(&gCrawlerScreenshot.rcBox,0,0,nWidth,500);*/
		m_StaticHint.SetWindowText(_T("���ö�ȡʧ�ܣ��������������ݣ�"));
	} else {
		SetDlgItemText(IDC_EDIT_CLASS_NAME,gCrawlerScreenshot.szClassName);
		SetDlgItemText(IDC_EDIT_TITLE,gCrawlerScreenshot.szTitle);
	}
	PostMessage(WM_MESSAGE_EVENT, EVENT_WORKER_INITIALIZED, 0);

	if (!IsWindow(m_hFromWnd)) {
		LPCTSTR lpClassName = gCrawlerScreenshot.szClassName;
		LPCTSTR lpTitle = gCrawlerScreenshot.szTitle;
		if (*lpClassName == 0) {
			lpClassName = NULL;
		}

		if (lpClassName == NULL && *lpTitle == 0) {
			//MessageBox(_T("�������òɼ�����"), gProductName, MB_OK);
			return FALSE;
		}

		m_hFromWnd = ::FindWindow(lpClassName, lpTitle);
		if (m_hFromWnd == NULL) {
			//MessageBox(_T("δ�ҵ�������������Ҫ�ɼ��ĳ���"), gProductName, MB_OK);
			return FALSE;
		}
	}
	PostMessage(WM_MESSAGE_EVENT, EVENT_PROCESS_ATTACH, (LPARAM)m_hFromWnd);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

//��Ϣ����
BOOL CCrawlerDialog::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if ((pMsg->message==WM_KEYDOWN)&&((pMsg->wParam==VK_ESCAPE)||(pMsg->wParam==VK_RETURN)))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

void CCrawlerDialog::OnTimer(UINT nIDEvent) 
{
	if (IDI_TIMEER_DRAG == nIDEvent) {

		HWND hFromWnd = GetWindowFromPoint();
		if (hFromWnd == NULL) {
			return ;
		}
		HWND hDesktopWnd = ::GetDesktopWindow();
		HDC hDC = ::GetWindowDC(hDesktopWnd);
		// ��ȡ�ؼ�����
		CRect rc;
		::GetWindowRect(hFromWnd, &rc);
		// ������ʹ��hMemDC����
		 //�����»���,����DeskDC
		HPEN newPen = ::CreatePen(0, 3, RGB(125,0,125));
		int oldRop2 = SetROP2(hDC, R2_NOTXORPEN);

		if( rc.left < 0 ) rc.left = 0;
		if (rc.top < 0 ) rc.top = 0;

		// �����ڴ��е�ͼ��������Ļ����ʾ
		HGDIOBJ oldPen = ::SelectObject(hDC, newPen);
		//��ָʾ������Χ��ʾ��˸����
		::Rectangle(hDC, rc.left, rc.top, rc.right, rc.bottom);

		//������˸ʱ����
		Sleep(400);
		::Rectangle( hDC, rc.left, rc.top, rc.right, rc.bottom);

		// �ͷ���Դ
		::SetROP2(hDC, oldRop2);
		::SelectObject( hDC, oldPen);
		::DeleteObject(newPen);

		::ReleaseDC( hDesktopWnd, hDC);
		//
	}

	__super::OnTimer(nIDEvent);
}

//�¼���Ϣ
LRESULT CCrawlerDialog::OnMessageEvent(WPARAM wParam, LPARAM lParam)
{
	//�¼�����
	switch (wParam)
	{
	case EVENT_DRAG_STARTED:			//��ȡ�����¼�
		{
			SetTimer(IDI_TIMEER_DRAG, 800, NULL);
			return 0L;
		}
	case EVENT_OPEN_PROCESS_ATTACH: {
			CProcessDialog processDialog;
			processDialog.DoModal();
		}
	case EVENT_DRAG_ENDED:		//��ȡ�����¼�
			KillTimer(IDI_TIMEER_DRAG);
	case EVENT_PROCESS_ATTACH: {
			m_hFromWnd = (HWND)lParam;
			if (!::IsWindow(m_hFromWnd)) {
				return 0L;
			}

			::GetWindowText(m_hFromWnd,gCrawlerScreenshot.szTitle,_cc_countof(gCrawlerScreenshot.szTitle));
			//ֱ��ʹ��ȫ�ֱ���gCrawlerScreenshot.szClassName ������Ч���ڴ����
			//����ʹ����ʱ������ȡ����
			TCHAR szClassName[128];
			GetClassName(m_hFromWnd,szClassName,_cc_countof(szClassName));
			_tcsncpy_s(gCrawlerScreenshot.szClassName, szClassName, _cc_countof(gCrawlerScreenshot.szClassName));
			gCrawlerScreenshot.szClassName[_cc_countof(gCrawlerScreenshot.szClassName) - 1] = 0;

			SetDlgItemText(IDC_EDIT_CLASS_NAME,gCrawlerScreenshot.szClassName);
			SetDlgItemText(IDC_EDIT_TITLE,gCrawlerScreenshot.szTitle);

			SaveConfigFile();

			m_ListCtrl.DeleteAllItems();
			GetWindowThreadProcessId(m_hFromWnd,&m_dwProcessID);

			EnumProcessModule(m_dwProcessID);

			_cc_rbtree_iterator_t *v = NULL;
			for (v = FirstProcessModule(); v != NULL; v = _cc_rb_next(v)) {
				m_ListCtrl.InsertDataItem(_cc_upcast(v, tagProcessModule, node));
			}

			CButton *btStarting = ((CButton*)GetDlgItem(IDC_BUTTON_RUNNING));
			if (!btStarting->IsWindowEnabled()) {
				btStarting->EnableWindow(TRUE);
			}
			return 0L;
		}
	case EVENT_SETTING_CLOSE: {
			SaveConfigFile();
		return 0L;
		}
	case EVENT_WORKER_INITIALIZED: {
			INT nErrorCode = (INT)lParam;
			if (nErrorCode != 0) {
				return 0L;
			}
			//����
			((CButton*)GetDlgItem(IDCANCEL))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_BUTTON_SETTING))->EnableWindow(TRUE);
			m_bInitialized = TRUE;

		return 0L;
		}
	case EVENT_WORKER_CLOSEED: {
			((CButton*)GetDlgItem(IDC_BUTTON_RUNNING))->SetWindowText(_T("��ʼ�ɼ�"));
			GetDlgItem(IDC_BUTTON_SETTING)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_RUNNING)->EnableWindow(TRUE);
			m_cbStatus = _STATUS_STOPED_;
		return 0L;
		}
	}

	return 0L; 
}

//ͼ����Ϣ
LRESULT CCrawlerDialog::OnMessageTrayIcon(WPARAM wParam, LPARAM lParam)
{
	//�¼�����
	switch (lParam)
	{
	case WM_LBUTTONDOWN:		//��굥��
	case WM_LBUTTONDBLCLK:		//���˫��
		{
			OnEventShow();
			return 1;
		}
	case WM_RBUTTONDOWN:		//��굥��
	case WM_RBUTTONDBLCLK:		//���˫��
		{
			//�����˵�
			
			CSkinMenu Menu;
			Menu.CreateMenu();

			Menu.AppendMenu(IDM_ABOUTBOX,TEXT("����..."));
			//����˵�
			Menu.AppendSeparator();
			Menu.AppendMenu(IDM_SHOW,TEXT("��ʾ����"));
			Menu.AppendMenu(IDM_CLOSE,TEXT("�˳�����"));

			//��ʾ�˵�
			CPoint MousePoint;
			GetCursorPos(&MousePoint);
			Menu.TrackPopupMenu(MousePoint.x,MousePoint.y,this);
			return 1;
		}
	}

	return 0;
}
void CCrawlerDialog::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
		OnEventAboutBox();
	} else if (nID == SC_CLOSE && m_cbStatus != _STATUS_STOPED_){
		if (!m_bTipClose) {
			MessageBox(_T("���������ں�̨��������Ҫ�˳������������˳�����ť���������½�ͼ���˳�����"), gProductName, MB_OK);
			m_bTipClose = TRUE;
		}
		ShowWindow(SW_HIDE);
		return;
	}

	__super::OnSysCommand(nID, lParam);
}

//˫���б�
void CCrawlerDialog::OnNMDBLClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	TCHAR szBuffer[1024];
	NM_LISTVIEW *pNMListCtrl = (NM_LISTVIEW *)pNMHDR;
	//��¼ѡ�е���
	INT iItem = pNMListCtrl->iItem;
	//��¼ѡ�е���
	INT iSubItem = pNMListCtrl->iSubItem;
	if (pNMListCtrl->iItem != -1) {
		m_ListCtrl.GetItemText(iItem, iSubItem, szBuffer, _cc_countof(szBuffer));
		_cc_set_clipboard_text(szBuffer);
	}
}

//ȡ������
void CCrawlerDialog::OnCancel(){
	if (!m_bInitialized) {
		MessageBox(_T("���ڳ�ʼ�������Ժ��ٹرա�"), gProductName, MB_OK);
		return ;
	}

	if (m_cbStatus != _STATUS_STOPED_) {
		MessageBox(_T("����ֹͣ�ɼ������Ժ��ٹرա�"), gProductName, MB_OK);
		return ;
	}

	__super::OnCancel();
}
//��ʾ����
void CCrawlerDialog::OnEventAboutBox()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}
//��ʾ����
void CCrawlerDialog::OnEventShow()
{
	ShowWindow(SW_RESTORE);
}

//�˳�����
void CCrawlerDialog::OnEventClose()
{
	__super::OnOK();
}
// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCrawlerDialog::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		__super::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCrawlerDialog::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CCrawlerDialog::SaveConfigFile()
{
	BYTE nFileVersion = 0;
	BYTE cbBuffer[sizeof(tagCrawlerScreenshot)];

	nFileVersion = _encryptMapping(cbBuffer, (byte_t*)&gCrawlerScreenshot, sizeof(tagCrawlerScreenshot));

	FILE *fp = _tfopen(m_szPathFile, _T("wb"));
	if (fp == NULL) {
		return FALSE;
	}

	fwrite(&nFileVersion, sizeof(byte_t), 1, fp);
	fwrite(cbBuffer, sizeof(byte_t), sizeof(tagCrawlerScreenshot), fp);
	fclose(fp);

	return TRUE;
}

BOOL CCrawlerDialog::ReadDBConfigFile(LPCTSTR lpPath) {
	TCHAR szPathFile[_CC_MAX_PATH_];

	_sntprintf_s(szPathFile, _cc_countof(szPathFile), _T("%s\\Crowler.ini"), lpPath);
	if (GetPrivateProfileString(TEXT("MYSQL"),
		TEXT("DBHost"),
		TEXT("127.0.0.1"),
		gCrawlerConfig.szHost,
		_cc_countof(gCrawlerConfig.szHost),
		szPathFile) <= 0) {
			return FALSE;
	}

	if (GetPrivateProfileString(TEXT("MYSQL"),
		TEXT("DBName"),
		TEXT("Crawler"),
		gCrawlerConfig.szDBName,
		_cc_countof(gCrawlerConfig.szDBName),
		szPathFile) <= 0) {
			return FALSE;
	}

	if (GetPrivateProfileString(TEXT("MYSQL"),
		TEXT("DBUser"),
		TEXT("root"),
		gCrawlerConfig.szUser,
		_cc_countof(gCrawlerConfig.szUser),
		szPathFile) <= 0) {
			return FALSE;
	}

	if (GetPrivateProfileString(TEXT("MYSQL"),
		TEXT("DBPassword"),
		TEXT(""),
		gCrawlerConfig.szPassword,
		_cc_countof(gCrawlerConfig.szPassword),
		szPathFile) <= 0) {
			return FALSE;
	}

	gCrawlerConfig.cbPlatform = (BYTE)GetPrivateProfileInt(TEXT("MYSQL"), TEXT("Platform"), 1, szPathFile);
	gCrawlerConfig.wPort = (USHORT)GetPrivateProfileInt(TEXT("MYSQL"), TEXT("DBPort"), 3306, szPathFile);

	gCrawlerConfig.cbSuccess = 1;

	return TRUE;
}

BOOL CCrawlerDialog::ReadConfigFile()
{
	BYTE nFileVersion = 0;
	BYTE cbBuffer[sizeof(gCrawlerScreenshot)];
	size_t nReadSize = 0;

	FILE *fp = _tfopen(m_szPathFile, _T("rb"));
	if (fp == NULL) {
		return FALSE;
	}

	if (fread(&nFileVersion, sizeof(BYTE), 1, fp) != 1) {
		fclose(fp);
		return FALSE;
	}

	nReadSize = fread(&cbBuffer, sizeof(BYTE), sizeof(tagCrawlerScreenshot), fp);
	if (nReadSize < sizeof(tagCrawlerScreenshot)) {
		return FALSE;
	}

	fclose(fp);

	if (_decryptMapping((byte_t*)(&gCrawlerScreenshot), cbBuffer, sizeof(tagCrawlerScreenshot)) != nFileVersion) {
		_CC_ERROR_LOG(_T("Could't read config file: 0x%0.2x"), nFileVersion);
		return FALSE;
	}
	return TRUE;
}
/*
struct tagFind {
	TCHAR *lpName;
	INT length;
	HWND hWnd;
};

HANDLE	hProcess = NULL;
HWND hWnd = NULL;

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam) 
{
	struct tagFind *find = (struct tagFind*)lParam;
	TCHAR lpString[1024];

	GetWindowText(hwnd, lpString, 1024);
	//GetClassName(hwnd, ClassString, 1024);
	if (_tcsncmp(find->lpName, lpString, find->length) == 0) {
		find->hWnd = hwnd;
		return FALSE;
	}
	
	return TRUE;
}

BOOL EnumChildWindow(HWND hWnd, struct tagFind *find) {
	HWND hWndChild = ::GetWindow(hWnd, GW_CHILD);
	while(hWnd) {
		TCHAR lpString[1024];

		GetWindowText(hWndChild, lpString, 1024);
		//GetClassName(hwnd, ClassString, 1024);
		//OutputDebugString(lpString);
		if (_tcsncmp(find->lpName, lpString, find->length) == 0) {
			find->hWnd = hWndChild;
			return TRUE;
		}
		hWndChild = ::GetWindow(hWndChild, GW_HWNDNEXT);
	}
	return FALSE;
}

BOOL CCrawlerDialog::GetCurrencyData() 
{
	// TODO: Add your control notification handler code here
	m_ListCtrl.DeleteAllItems();
	// Find window handle
	struct tagFind find;
	find.lpName = _T("�г�����");
	find.length = _tcslen(find.lpName);
	find.hWnd = NULL;
	hWnd = ::FindWindow(_T("MetaQuotes::MetaTrader::4.00"),NULL); 
	if (EnumChildWindow(hWnd, &find) == FALSE) {
		//MessageBox(_T("����û���ҵ� ��"));
		return FALSE;
	}
	hWnd = find.hWnd;
	find.hWnd = NULL;
	;
	if (EnumChildWindow(hWnd, &find) == FALSE) {
		//MessageBox(_T("����û���ҵ� ��"));
		return FALSE;
	}

	hWnd = find.hWnd;
	hWnd = ::FindWindowEx(hWnd,0,_T("SysListView32"),NULL);
	if(hWnd == NULL) {
		//MessageBox(_T("����û���ҵ� ��"));
		return FALSE;
	}

	DWORD	pid;
	// Get process pid.
	GetWindowThreadProcessId(hWnd, &pid);
	hProcess = OpenProcess(PROCESS_VM_OPERATION|PROCESS_VM_READ|
		PROCESS_VM_WRITE|PROCESS_QUERY_INFORMATION, FALSE, pid);
	if (hProcess) {
		// Get the ListView lines.
		int count = (int)::SendMessage(hWnd,LVM_GETITEMCOUNT,0,0);
		int i,nItem;

		LVITEM	lvi, *_lvi;
		TCHAR	firstitem[512], secitem[512],thirditem[512],fourthitem[512];
		TCHAR	*_firstitem, *_secitem,*_thirditem,*_fourthitem;

		if (count <= 0)
			return FALSE;

		memset(firstitem,0,512);
		memset(secitem,0,512);
		memset(thirditem,0,512);
		memset(fourthitem,0,512);

		_lvi = (LVITEM*)VirtualAllocEx(hProcess, NULL, sizeof(LVITEM),MEM_COMMIT, PAGE_READWRITE);
		_firstitem = (TCHAR*)VirtualAllocEx(hProcess, NULL, 512, MEM_COMMIT,PAGE_READWRITE);
		_secitem  = (TCHAR*)VirtualAllocEx(hProcess, NULL, 512, MEM_COMMIT,PAGE_READWRITE);
		_thirditem = (TCHAR*)VirtualAllocEx(hProcess, NULL, 512, MEM_COMMIT,PAGE_READWRITE);
		_fourthitem = (TCHAR*)VirtualAllocEx(hProcess,NULL, 512, MEM_COMMIT,PAGE_READWRITE);
		lvi.cchTextMax=512;

		// Here use degression , order with the task manager
		for(i = count; i >= 0; i--) {
			// Read first item data
			lvi.iSubItem = 0;
			lvi.pszText = _firstitem;
			WriteProcessMemory(hProcess, _lvi, &lvi, sizeof(LVITEM), NULL);
			::SendMessage(hWnd, LVM_GETITEMTEXT, (WPARAM)i, (LPARAM)_lvi);


			// Read second item data
			lvi.iSubItem = 1;
			lvi.pszText = _secitem;
			WriteProcessMemory(hProcess, _lvi, &lvi, sizeof(LVITEM), NULL);
			::SendMessage(hWnd, LVM_GETITEMTEXT, (WPARAM)i, (LPARAM)_lvi);


			// Read 3rd item data
			lvi.iSubItem = 2;
			lvi.pszText = _thirditem;
			WriteProcessMemory(hProcess, _lvi, &lvi, sizeof(LVITEM), NULL);
			::SendMessage(hWnd, LVM_GETITEMTEXT, (WPARAM)i, (LPARAM)_lvi);


			// Read the hProcess memory.
			ReadProcessMemory(hProcess, _firstitem, firstitem, 512, NULL);
			ReadProcessMemory(hProcess, _secitem, secitem, 512, NULL);
			ReadProcessMemory(hProcess, _thirditem, thirditem, 512, NULL);

			// Add it into my ListCtrl.
			nItem = m_ListCtrl.InsertItem(0,firstitem);
			m_ListCtrl.SetItem(nItem,1,1,secitem,NULL,0,0,0);
			m_ListCtrl.SetItem(nItem,2,1,thirditem,NULL,0,0,0);
		}

		// Release memory
		VirtualFreeEx(hProcess, _lvi, 0, MEM_RELEASE);
		VirtualFreeEx(hProcess, _firstitem,  0, MEM_RELEASE);
		VirtualFreeEx(hProcess, _secitem,    0, MEM_RELEASE);
		VirtualFreeEx(hProcess, _thirditem,  0, MEM_RELEASE);
	}
	return TRUE;
}*/

void CCrawlerDialog::OnBnClickedSetting()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//GetCurrencyData();
	/*
	if (m_dwProcessID) {
		m_ReadMemoryDialog.Open(m_dwProcessID, this);
	}
	*/
	//m_ScreenshotWnd.Open(m_hFromWnd);
}

void CCrawlerDialog::OnBnClickedButtonRunning()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_bInitialized) {
		MessageBox(_T("���ڳ�ʼ�������Ժ�"), gProductName, MB_OK);
		return ;
	}

	if (m_cbStatus == _STATUS_STOPPING_) {
		return;
	}

	if (m_cbStatus == _STATUS_STARTED_) {
		m_cbStatus = _STATUS_STOPPING_;
		GetDlgItem(IDC_BUTTON_RUNNING)->SetWindowText(_T("����ֹͣ..."));
		GetDlgItem(IDC_BUTTON_RUNNING)->EnableWindow(FALSE);
	} else {
		GetDlgItem(IDC_BUTTON_RUNNING)->SetWindowText(_T("ֹͣ�ɼ�"));
		m_cbStatus = _STATUS_STARTED_;
		_cc_thread_start(doStartingWorker, this);
	}

	GetDlgItem(IDC_BUTTON_SETTING)->EnableWindow(m_cbStatus == _STATUS_STOPED_);
}
