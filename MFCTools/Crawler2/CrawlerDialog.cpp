
// CrawlerDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Crawler.h"
#include "CrawlerDialog.h"
#include "afxdialogex.h"
#include "Worker.h"
#include "Hook.h"
#include "ProcessDialog.h"
#include "SettingDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//1���Լ�����ϵͳ��WM_HOTKEY��Ϣ
#define ID_DOIT   0X6000

#define IDC_SYSTEM_TRAY_ICON		100									//����ͼ��

#define IDM_NULL_COMMAND (WM_USER + 1000)
#define IDM_SHOW (WM_USER + 1001)
#define IDM_CLOSE (WM_USER + 1002)
#define IDI_TIMEER_DRAG				1

TCHAR chHotKey = 0;
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
	m_bTipClose = FALSE;
	m_bInitialized = FALSE;
	m_cbStatus = _STATUS_STOPED_;
	m_hFromWnd = NULL;
	m_dwProcessID = 0;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CCrawlerDialog::~CCrawlerDialog()
{
	SaveConfigFile();
}

void CCrawlerDialog::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_HINT, m_StaticHint);
	DDX_Control(pDX, IDC_BUTTON_SETTING, m_btSetting);
	DDX_Control(pDX, IDC_BUTTON_RUNNING, m_btRuning);
	DDX_Control(pDX, IDC_EDIT_DELAY_TIME, m_editDelayTime);
    
	DDX_Control(pDX, IDCANCEL, m_btCancel);
}

BEGIN_MESSAGE_MAP(CCrawlerDialog, CSkinDialog)
	ON_WM_SYSCOMMAND()
	//ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()

	//�˵���Ϣ
	ON_COMMAND(IDM_SHOW,OnEventShow)
	ON_COMMAND(IDM_CLOSE,OnEventClose)
	ON_COMMAND(IDM_ABOUTBOX,OnEventAboutBox)

    //ע���ȼ��¼�
    ON_MESSAGE(WM_HOTKEY,OnHotKey)
	ON_MESSAGE(WM_TRAY_ICON, OnMessageTrayIcon)
	ON_MESSAGE(WM_MESSAGE_EVENT, OnMessageEvent)
	//ON_STN_CLICKED(IDC_PIC_DRAG, OnStnClickedPicDrag)
	ON_BN_CLICKED(IDC_BUTTON_SETTING, &CCrawlerDialog::OnBnClickedSetting)
	ON_BN_CLICKED(IDC_BUTTON_RUNNING, &CCrawlerDialog::OnBnClickedButtonRunning)
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
	gCrawlerConfig.cbSuccess = ReadConfigFile();
	
	CenterWindow();
	ShowWindow(SW_SHOWNOACTIVATE);

	m_StaticDrag.SubclassDlgItem(IDC_PIC_DRAG,this);
	m_StaticAttach.SubclassDlgItem(IDC_PIC_ATTACH,this);

	//��������
	m_SystemTrayIcon.InitTrayIcon(m_hWnd, IDC_SYSTEM_TRAY_ICON);
	m_SystemTrayIcon.ShowTrayIcon(m_hIcon, gProductName);

    TCHAR doits[] = {'D','P','A','B','G','H','Y','I','U','W','Z','F'};
    for (int i = 0; i < _cc_countof(doits); i++) {
        if (::RegisterHotKey(this->GetSafeHwnd(),ID_DOIT,MOD_CONTROL,doits[i])) {
            chHotKey = doits[i];
            break;
        }
    }

	if (!gCrawlerConfig.cbSuccess) {
        m_StaticHint.SetWindowText(_T("���ö�ȡʧ�ܣ��������������ݣ�"));
        SetDlgItemText(IDC_EDIT_CLASS_NAME,_T("Qt5QWindowIcon"));
        SetDlgItemText(IDC_EDIT_TITLE,_T("��ңģ����"));
	} else {
		SetDlgItemText(IDC_EDIT_CLASS_NAME,_T("Qt5QWindowIcon"));
		SetDlgItemText(IDC_EDIT_TITLE,_T("��ңģ����"));
        CString info;
        info.Format(_T("Ctrl + %c ����/ֹͣ"), chHotKey);
        m_StaticHint.SetWindowText(info);
        m_hFromWnd = ::FindWindow(_T("Qt5QWindowIcon"), _T("��ңģ����"));
	}

    if (gCrawlerConfig.delayTime <= 0) {
        gCrawlerConfig.delayTime = 1000;
    }
    SetDlgItemInt(IDC_EDIT_DELAY_TIME, gCrawlerConfig.delayTime);
    ((CButton*)GetDlgItem(IDC_CHECK_SOUND))->SetCheck(1);

	PostMessage(WM_MESSAGE_EVENT, EVENT_WORKER_INITIALIZED, 0);

	if (!IsWindow(m_hFromWnd)) {
		LPCTSTR lpClassName = _T("Qt5QWindowIcon");//gCrawlerConfig.szClassName;
		LPCTSTR lpTitle = _T("��ңģ����");//gCrawlerConfig.szTitle;
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

			::GetWindowText(m_hFromWnd,gCrawlerConfig.szTitle,_cc_countof(gCrawlerConfig.szTitle));
			//ֱ��ʹ��ȫ�ֱ���gCrawlerConfig.szClassName ������Ч���ڴ����
			//����ʹ����ʱ������ȡ����
			TCHAR szClassName[128];
			GetClassName(m_hFromWnd,szClassName,_cc_countof(szClassName));
			_tcsncpy_s(gCrawlerConfig.szClassName, szClassName, _cc_countof(gCrawlerConfig.szClassName));
			gCrawlerConfig.szClassName[_cc_countof(gCrawlerConfig.szClassName) - 1] = 0;

			SetDlgItemText(IDC_EDIT_CLASS_NAME,gCrawlerConfig.szClassName);
			SetDlgItemText(IDC_EDIT_TITLE,gCrawlerConfig.szTitle);

            m_btSetting.EnableWindow(TRUE);
            m_btRuning.EnableWindow(TRUE);

			SaveConfigFile();

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
			((CButton*)GetDlgItem(IDC_BUTTON_SETTING))->EnableWindow(FALSE);
			m_bInitialized = TRUE;

		return 0L;
		}
	case EVENT_WORKER_CLOSEED: {
			((CButton*)GetDlgItem(IDC_BUTTON_RUNNING))->SetWindowText(_T("��ʼ"));
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


void CCrawlerDialog::OnBnClickedSetting()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    CSettingDialog setting;
    if (!IsWindow(m_hFromWnd)) {
		MessageBox(_T("�������ô��ھ����"), gProductName, MB_OK);
        return;
    }
    setting.SetFromWnd(m_hFromWnd);
    setting.DoModal();
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

        PostMessage(WM_MESSAGE_EVENT, EVENT_WORKER_CLOSEED, (LPARAM)m_hFromWnd);
	} else {
        gCrawlerConfig.delayTime = GetDlgItemInt(IDC_EDIT_DELAY_TIME);
        if (gCrawlerConfig.delayTime <= 0) {
		    MessageBox(_T("�ӳ�ʱ�䲻��С��0��"), gProductName, MB_OK);
		    return ;
        }
		GetDlgItem(IDC_BUTTON_RUNNING)->SetWindowText(_T("ֹͣ"));
		m_cbStatus = _STATUS_STARTED_;
        _cc_thread_start(doStartingWorker, _T("Crawler"), this);
        ShowWindow(SW_HIDE);
	}

	GetDlgItem(IDC_BUTTON_SETTING)->EnableWindow(m_cbStatus == _STATUS_STOPED_);
}

//5��ʵ��OnHotKey��������������ȼ��¼���
LONG CCrawlerDialog::OnHotKey(WPARAM wPARAM, LPARAM lPARAM)
{
    OnBnClickedButtonRunning();
    return 0;
}