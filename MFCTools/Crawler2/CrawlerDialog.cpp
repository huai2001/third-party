
// CrawlerDialog.cpp : 实现文件
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

//1、自己定义系统的WM_HOTKEY消息
#define ID_DOIT   0X6000

#define IDC_SYSTEM_TRAY_ICON		100									//任务图标

#define IDM_NULL_COMMAND (WM_USER + 1000)
#define IDM_SHOW (WM_USER + 1001)
#define IDM_CLOSE (WM_USER + 1002)
#define IDI_TIMEER_DRAG				1

TCHAR chHotKey = 0;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CSkinDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CCrawlerDialog 对话框
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

	//菜单消息
	ON_COMMAND(IDM_SHOW,OnEventShow)
	ON_COMMAND(IDM_CLOSE,OnEventClose)
	ON_COMMAND(IDM_ABOUTBOX,OnEventAboutBox)

    //注册热键事件
    ON_MESSAGE(WM_HOTKEY,OnHotKey)
	ON_MESSAGE(WM_TRAY_ICON, OnMessageTrayIcon)
	ON_MESSAGE(WM_MESSAGE_EVENT, OnMessageEvent)
	//ON_STN_CLICKED(IDC_PIC_DRAG, OnStnClickedPicDrag)
	ON_BN_CLICKED(IDC_BUTTON_SETTING, &CCrawlerDialog::OnBnClickedSetting)
	ON_BN_CLICKED(IDC_BUTTON_RUNNING, &CCrawlerDialog::OnBnClickedButtonRunning)
END_MESSAGE_MAP()


// CCrawlerDialog 消息处理程序

BOOL CCrawlerDialog::OnInitDialog()
{
	__super::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	SetWindowText(gProductName);

	m_StaticHint.SetWindowText(_T("正在读取配置"));
	gCrawlerConfig.cbSuccess = ReadConfigFile();
	
	CenterWindow();
	ShowWindow(SW_SHOWNOACTIVATE);

	m_StaticDrag.SubclassDlgItem(IDC_PIC_DRAG,this);
	m_StaticAttach.SubclassDlgItem(IDC_PIC_ATTACH,this);

	//设置托盘
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
        m_StaticHint.SetWindowText(_T("配置读取失败，请重新配置数据！"));
        SetDlgItemText(IDC_EDIT_CLASS_NAME,_T("Qt5QWindowIcon"));
        SetDlgItemText(IDC_EDIT_TITLE,_T("逍遥模拟器"));
	} else {
		SetDlgItemText(IDC_EDIT_CLASS_NAME,_T("Qt5QWindowIcon"));
		SetDlgItemText(IDC_EDIT_TITLE,_T("逍遥模拟器"));
        CString info;
        info.Format(_T("Ctrl + %c 启用/停止"), chHotKey);
        m_StaticHint.SetWindowText(info);
        m_hFromWnd = ::FindWindow(_T("Qt5QWindowIcon"), _T("逍遥模拟器"));
	}

    if (gCrawlerConfig.delayTime <= 0) {
        gCrawlerConfig.delayTime = 1000;
    }
    SetDlgItemInt(IDC_EDIT_DELAY_TIME, gCrawlerConfig.delayTime);
    ((CButton*)GetDlgItem(IDC_CHECK_SOUND))->SetCheck(1);

	PostMessage(WM_MESSAGE_EVENT, EVENT_WORKER_INITIALIZED, 0);

	if (!IsWindow(m_hFromWnd)) {
		LPCTSTR lpClassName = _T("Qt5QWindowIcon");//gCrawlerConfig.szClassName;
		LPCTSTR lpTitle = _T("逍遥模拟器");//gCrawlerConfig.szTitle;
		if (*lpClassName == 0) {
			lpClassName = NULL;
		}

		if (lpClassName == NULL && *lpTitle == 0) {
			//MessageBox(_T("请先配置采集程序！"), gProductName, MB_OK);
			return FALSE;
		}

		m_hFromWnd = ::FindWindow(lpClassName, lpTitle);
		if (m_hFromWnd == NULL) {
			//MessageBox(_T("未找到，请先运行需要采集的程序！"), gProductName, MB_OK);
			return FALSE;
		}
	}
	PostMessage(WM_MESSAGE_EVENT, EVENT_PROCESS_ATTACH, (LPARAM)m_hFromWnd);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

//消息解释
BOOL CCrawlerDialog::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
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
		// 获取控件矩形
		CRect rc;
		::GetWindowRect(hFromWnd, &rc);
		// 在这里使用hMemDC绘制
		 //建立新画笔,载入DeskDC
		HPEN newPen = ::CreatePen(0, 3, RGB(125,0,125));
		int oldRop2 = SetROP2(hDC, R2_NOTXORPEN);

		if( rc.left < 0 ) rc.left = 0;
		if (rc.top < 0 ) rc.top = 0;

		// 将在内存中的图拷贝到屏幕上显示
		HGDIOBJ oldPen = ::SelectObject(hDC, newPen);
		//在指示窗口周围显示闪烁矩形
		::Rectangle(hDC, rc.left, rc.top, rc.right, rc.bottom);

		//设置闪烁时间间隔
		Sleep(400);
		::Rectangle( hDC, rc.left, rc.top, rc.right, rc.bottom);

		// 释放资源
		::SetROP2(hDC, oldRop2);
		::SelectObject( hDC, oldPen);
		::DeleteObject(newPen);

		::ReleaseDC( hDesktopWnd, hDC);
		//
	}

	__super::OnTimer(nIDEvent);
}

//事件消息
LRESULT CCrawlerDialog::OnMessageEvent(WPARAM wParam, LPARAM lParam)
{
	//事件处理
	switch (wParam)
	{
	case EVENT_DRAG_STARTED:			//获取窗口事件
		{
			SetTimer(IDI_TIMEER_DRAG, 800, NULL);
			return 0L;
		}
	case EVENT_OPEN_PROCESS_ATTACH: {
			CProcessDialog processDialog;
			processDialog.DoModal();
		}
	case EVENT_DRAG_ENDED:		//获取窗口事件
			KillTimer(IDI_TIMEER_DRAG);
	case EVENT_PROCESS_ATTACH: {
			m_hFromWnd = (HWND)lParam;
			if (!::IsWindow(m_hFromWnd)) {
				return 0L;
			}

			::GetWindowText(m_hFromWnd,gCrawlerConfig.szTitle,_cc_countof(gCrawlerConfig.szTitle));
			//直接使用全局变量gCrawlerConfig.szClassName 导致无效的内存访问
			//必须使用零时变量获取类名
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
			//启用
			((CButton*)GetDlgItem(IDCANCEL))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_BUTTON_SETTING))->EnableWindow(FALSE);
			m_bInitialized = TRUE;

		return 0L;
		}
	case EVENT_WORKER_CLOSEED: {
			((CButton*)GetDlgItem(IDC_BUTTON_RUNNING))->SetWindowText(_T("开始"));
			GetDlgItem(IDC_BUTTON_SETTING)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_RUNNING)->EnableWindow(TRUE);
			m_cbStatus = _STATUS_STOPED_;
		return 0L;
		}
	}

	return 0L; 
}

//图标消息
LRESULT CCrawlerDialog::OnMessageTrayIcon(WPARAM wParam, LPARAM lParam)
{
	//事件处理
	switch (lParam)
	{
	case WM_LBUTTONDOWN:		//鼠标单击
	case WM_LBUTTONDBLCLK:		//鼠标双击
		{
			OnEventShow();
			return 1;
		}
	case WM_RBUTTONDOWN:		//鼠标单击
	case WM_RBUTTONDBLCLK:		//鼠标双击
		{
			//创建菜单
			
			CSkinMenu Menu;
			Menu.CreateMenu();

			Menu.AppendMenu(IDM_ABOUTBOX,TEXT("关于..."));
			//构造菜单
			Menu.AppendSeparator();
			Menu.AppendMenu(IDM_SHOW,TEXT("显示界面"));
			Menu.AppendMenu(IDM_CLOSE,TEXT("退出程序"));

			//显示菜单
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
			MessageBox(_T("窗口隐藏在后台工作，如要退出程序请点击“退出”按钮，或者右下角图标退出程序"), gProductName, MB_OK);
			m_bTipClose = TRUE;
		}
		ShowWindow(SW_HIDE);
		return;
	}

	__super::OnSysCommand(nID, lParam);
}

//取消函数
void CCrawlerDialog::OnCancel(){
	if (!m_bInitialized) {
		MessageBox(_T("正在初始化，请稍后再关闭。"), gProductName, MB_OK);
		return ;
	}

	if (m_cbStatus != _STATUS_STOPED_) {
		MessageBox(_T("请先停止采集，请稍后再关闭。"), gProductName, MB_OK);
		return ;
	}

	__super::OnCancel();
}
//显示关于
void CCrawlerDialog::OnEventAboutBox()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}
//显示窗口
void CCrawlerDialog::OnEventShow()
{
	ShowWindow(SW_RESTORE);
}

//退出程序
void CCrawlerDialog::OnEventClose()
{
	__super::OnOK();
}
// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCrawlerDialog::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		__super::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCrawlerDialog::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCrawlerDialog::OnBnClickedSetting()
{
	// TODO: 在此添加控件通知处理程序代码
    CSettingDialog setting;
    if (!IsWindow(m_hFromWnd)) {
		MessageBox(_T("请先设置窗口句柄！"), gProductName, MB_OK);
        return;
    }
    setting.SetFromWnd(m_hFromWnd);
    setting.DoModal();
}

void CCrawlerDialog::OnBnClickedButtonRunning()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_bInitialized) {
		MessageBox(_T("正在初始化，请稍后！"), gProductName, MB_OK);
		return ;
	}

	if (m_cbStatus == _STATUS_STOPPING_) {
		return;
	}

	if (m_cbStatus == _STATUS_STARTED_) {
		m_cbStatus = _STATUS_STOPPING_;
		GetDlgItem(IDC_BUTTON_RUNNING)->SetWindowText(_T("正在停止..."));
		GetDlgItem(IDC_BUTTON_RUNNING)->EnableWindow(FALSE);

        PostMessage(WM_MESSAGE_EVENT, EVENT_WORKER_CLOSEED, (LPARAM)m_hFromWnd);
	} else {
        gCrawlerConfig.delayTime = GetDlgItemInt(IDC_EDIT_DELAY_TIME);
        if (gCrawlerConfig.delayTime <= 0) {
		    MessageBox(_T("延迟时间不能小于0！"), gProductName, MB_OK);
		    return ;
        }
		GetDlgItem(IDC_BUTTON_RUNNING)->SetWindowText(_T("停止"));
		m_cbStatus = _STATUS_STARTED_;
        _cc_thread_start(doStartingWorker, _T("Crawler"), this);
        ShowWindow(SW_HIDE);
	}

	GetDlgItem(IDC_BUTTON_SETTING)->EnableWindow(m_cbStatus == _STATUS_STOPED_);
}

//5、实现OnHotKey方法，处理具体热键事件：
LONG CCrawlerDialog::OnHotKey(WPARAM wPARAM, LPARAM lPARAM)
{
    OnBnClickedButtonRunning();
    return 0;
}