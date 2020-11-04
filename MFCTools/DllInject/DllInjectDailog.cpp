
// DllInjectDailog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DllInject.h"
#include "DllInjectDailog.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define IDM_INJECT_DLL (WM_USER + 100)
#define IDM_UNINJECT_DLL (WM_USER + 101)
#define IDM_REFRESH_PROCESS (WM_USER + 102)
#define IDM_REFRESH_PROCESS_MODULE (WM_USER + 103)

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CSkinDialog
{
public:
	CAboutDlg();
	CSkinButton m_OK;
// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	BOOL OnInitDialog();
// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CSkinDialog(CAboutDlg::IDD)
{

}

// CAboutDlg ��Ϣ�������
BOOL CAboutDlg::OnInitDialog()
{
	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(hIcon, FALSE);		// ����Сͼ��

	SetWindowText(_T("����"));

	return __super::OnInitDialog();
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_OK);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CSkinDialog)
END_MESSAGE_MAP()


// CDllInjectDailog �Ի���




CDllInjectDailog::CDllInjectDailog(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CDllInjectDailog::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDllInjectDailog::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROCESS_LIST, m_ProcessListCtrl);
	DDX_Control(pDX, IDC_PROCESS_MODULE_LIST, m_ProcessModuleListCtrl);
}

BEGIN_MESSAGE_MAP(CDllInjectDailog, CSkinDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_CLICK, IDC_PROCESS_LIST, OnLeftClickProcessList)
	ON_NOTIFY(NM_RCLICK, IDC_PROCESS_LIST, OnRightClickProcessList)
	ON_NOTIFY(NM_RCLICK, IDC_PROCESS_MODULE_LIST, OnRightClickProcessModuleList)
	//ON_WM_SIZE()
END_MESSAGE_MAP()


// CDllInjectDailog ��Ϣ�������
BOOL CDllInjectDailog::OnInitDialog()
{
	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

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


	SetWindowText(_T("DLL ע�빤��"));
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_ProcessListCtrl.InitView();
	m_ProcessModuleListCtrl.InitView();
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDllInjectDailog::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.Create(CAboutDlg::IDD, this);
		//dlgAbout.DoModal();
	}
	else
	{
		__super::OnSysCommand(nID, lParam);
	}
}

//�滭��Ϣ
VOID CDllInjectDailog::OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight)
{
	CRect rcList;
	COLORREF crBorder = RGB(0,96,159);
	m_ProcessListCtrl.GetWindowRect(&rcList);
	ScreenToClient(&rcList);
	rcList.InflateRect(1,1,1,1);
	pDC->Draw3dRect(rcList,crBorder,crBorder);

	m_ProcessModuleListCtrl.GetWindowRect(&rcList);
	ScreenToClient(&rcList);
	rcList.InflateRect(1,1,1,1);
	pDC->Draw3dRect(rcList,crBorder,crBorder);
	
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDllInjectDailog::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


tagProcessData *pProcessData = NULL;
tagProcessModule *pProcessModule = NULL;

//�����
BOOL CDllInjectDailog::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID = LOWORD(wParam);
	
	switch(nCommandID)
	{
	case IDM_ABOUTBOX: {
			CAboutDlg dlgAbout;
			dlgAbout.DoModal();
		}
		return TRUE;
	case IDM_INJECT_DLL:{
		if (pProcessData == NULL) {
			return TRUE;
		}

		m_InjectDialog.SetProcessData(pProcessData);
		//m_InjectDialog.DoModal();
		if (m_InjectDialog.GetSafeHwnd()==NULL) {
			m_InjectDialog.Create(CInjectDialog::IDD, GetDesktopWindow());
		}
		//��ʾ����
		m_InjectDialog.ShowWindow(SW_SHOW);
		m_InjectDialog.SetForegroundWindow();
		}
		return TRUE;
	case IDM_UNINJECT_DLL: {
			if (pProcessData == NULL || pProcessModule == NULL) {
				return TRUE;
			}

			if (unInjectDLL(pProcessData->dwProcessID, pProcessModule->dwBaseAddr)) {
				AfxMessageBox(_T("ж�سɹ�"));
			}
		}
		return TRUE;
	case IDM_REFRESH_PROCESS: {
			m_ProcessListCtrl.Refresh();
		}
		return TRUE;
	case IDM_REFRESH_PROCESS_MODULE: {
		if (pProcessData) {
				m_ProcessModuleListCtrl.Refresh(pProcessData->dwProcessID);
			}
		}
		return TRUE;
	}
	
	//�Զ���˵��¼�
	return __super::OnCommand(wParam,lParam);
}

//�һ��б�
void CDllInjectDailog::OnLeftClickProcessList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if(pNMLV->iItem == -1) {
		*pResult = 0;
		return ;
	}
	DWORD_PTR dwData = (DWORD_PTR)m_ProcessListCtrl.GetItemData(pNMLV->iItem);

	//��������
	if(dwData == -1) {
		return;
	}


	pProcessData = (tagProcessData*)dwData;

	m_ProcessModuleListCtrl.Refresh(pProcessData->dwProcessID);

}

//����б�
void CDllInjectDailog::OnRightClickProcessList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if(pNMLV->iItem == -1) {
		*pResult = 0;
		return ;
	}
	DWORD_PTR dwData = (DWORD_PTR)m_ProcessListCtrl.GetItemData(pNMLV->iItem);

	//��������
	if(dwData == -1) {
		return;
	}
	
	pProcessData = (tagProcessData*)dwData;
	//����˵�
	CSkinMenu Menu;
	Menu.CreateMenu();

	Menu.AppendMenu(IDM_ABOUTBOX,TEXT("����..."));
	//����˵�
	Menu.AppendSeparator();
	Menu.AppendMenu(IDM_INJECT_DLL,TEXT("ע��DLL"));
	Menu.AppendSeparator();
	Menu.AppendMenu(IDM_REFRESH_PROCESS,TEXT("ˢ��"));

	//��ʾ�˵�
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	Menu.TrackPopupMenu(MousePoint.x,MousePoint.y,this);

	*pResult = 0;
}

//����б�
void CDllInjectDailog::OnRightClickProcessModuleList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if(pNMLV->iItem == -1) {
		*pResult = 0;
		return ;
	}
	DWORD_PTR dwData = (DWORD_PTR)m_ProcessModuleListCtrl.GetItemData(pNMLV->iItem);

	//��������
	if(dwData == -1) {
		return;
	}

	pProcessModule = (tagProcessModule*)dwData;
	//����˵�
	CSkinMenu Menu;
	Menu.CreateMenu();

	Menu.AppendMenu(IDM_ABOUTBOX,TEXT("����..."));
	//����˵�
	Menu.AppendSeparator();
	Menu.AppendMenu(IDM_UNINJECT_DLL,TEXT("ж��DLL"));
	Menu.AppendSeparator();
	Menu.AppendMenu(IDM_REFRESH_PROCESS_MODULE,TEXT("ˢ��"));

	//��ʾ�˵�
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	Menu.TrackPopupMenu(MousePoint.x,MousePoint.y,this);

	*pResult = 0;
}
