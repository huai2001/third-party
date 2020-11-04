
// DllInjectDailog.cpp : 实现文件
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

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CSkinDialog
{
public:
	CAboutDlg();
	CSkinButton m_OK;
// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL OnInitDialog();
// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CSkinDialog(CAboutDlg::IDD)
{

}

// CAboutDlg 消息处理程序
BOOL CAboutDlg::OnInitDialog()
{
	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(hIcon, TRUE);			// 设置大图标
	SetIcon(hIcon, FALSE);		// 设置小图标

	SetWindowText(_T("关于"));

	return __super::OnInitDialog();
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_OK);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CSkinDialog)
END_MESSAGE_MAP()


// CDllInjectDailog 对话框




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


// CDllInjectDailog 消息处理程序
BOOL CDllInjectDailog::OnInitDialog()
{
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

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


	SetWindowText(_T("DLL 注入工具"));
	// TODO: 在此添加额外的初始化代码
	m_ProcessListCtrl.InitView();
	m_ProcessModuleListCtrl.InitView();
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

//绘画消息
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

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDllInjectDailog::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


tagProcessData *pProcessData = NULL;
tagProcessModule *pProcessModule = NULL;

//命令函数
BOOL CDllInjectDailog::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
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
		//显示窗口
		m_InjectDialog.ShowWindow(SW_SHOW);
		m_InjectDialog.SetForegroundWindow();
		}
		return TRUE;
	case IDM_UNINJECT_DLL: {
			if (pProcessData == NULL || pProcessModule == NULL) {
				return TRUE;
			}

			if (unInjectDLL(pProcessData->dwProcessID, pProcessModule->dwBaseAddr)) {
				AfxMessageBox(_T("卸载成功"));
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
	
	//自定义菜单事件
	return __super::OnCommand(wParam,lParam);
}

//右击列表
void CDllInjectDailog::OnLeftClickProcessList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if(pNMLV->iItem == -1) {
		*pResult = 0;
		return ;
	}
	DWORD_PTR dwData = (DWORD_PTR)m_ProcessListCtrl.GetItemData(pNMLV->iItem);

	//查找数据
	if(dwData == -1) {
		return;
	}


	pProcessData = (tagProcessData*)dwData;

	m_ProcessModuleListCtrl.Refresh(pProcessData->dwProcessID);

}

//左击列表
void CDllInjectDailog::OnRightClickProcessList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if(pNMLV->iItem == -1) {
		*pResult = 0;
		return ;
	}
	DWORD_PTR dwData = (DWORD_PTR)m_ProcessListCtrl.GetItemData(pNMLV->iItem);

	//查找数据
	if(dwData == -1) {
		return;
	}
	
	pProcessData = (tagProcessData*)dwData;
	//构造菜单
	CSkinMenu Menu;
	Menu.CreateMenu();

	Menu.AppendMenu(IDM_ABOUTBOX,TEXT("关于..."));
	//构造菜单
	Menu.AppendSeparator();
	Menu.AppendMenu(IDM_INJECT_DLL,TEXT("注入DLL"));
	Menu.AppendSeparator();
	Menu.AppendMenu(IDM_REFRESH_PROCESS,TEXT("刷新"));

	//显示菜单
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	Menu.TrackPopupMenu(MousePoint.x,MousePoint.y,this);

	*pResult = 0;
}

//左击列表
void CDllInjectDailog::OnRightClickProcessModuleList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if(pNMLV->iItem == -1) {
		*pResult = 0;
		return ;
	}
	DWORD_PTR dwData = (DWORD_PTR)m_ProcessModuleListCtrl.GetItemData(pNMLV->iItem);

	//查找数据
	if(dwData == -1) {
		return;
	}

	pProcessModule = (tagProcessModule*)dwData;
	//构造菜单
	CSkinMenu Menu;
	Menu.CreateMenu();

	Menu.AppendMenu(IDM_ABOUTBOX,TEXT("关于..."));
	//构造菜单
	Menu.AppendSeparator();
	Menu.AppendMenu(IDM_UNINJECT_DLL,TEXT("卸载DLL"));
	Menu.AppendSeparator();
	Menu.AppendMenu(IDM_REFRESH_PROCESS_MODULE,TEXT("刷新"));

	//显示菜单
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	Menu.TrackPopupMenu(MousePoint.x,MousePoint.y,this);

	*pResult = 0;
}
