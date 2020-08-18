
// CrawlerDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "Crawler.h"
#include "CrawlerDialog.h"
#include "afxdialogex.h"
#include "Screenshots.h"
#include "Worker.h"
#include <cc/json/json.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IDC_SYSTEM_TRAY_ICON		100									//任务图标

#define IDM_NULL_COMMAND (WM_USER + 1000)
#define IDM_SHOW (WM_USER + 1001)
#define IDM_CLOSE (WM_USER + 1002)

#define IDI_TIMEER_DRAG				1

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCrawlerDialog 对话框
CCrawlerDialog::CCrawlerDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCrawlerDialog::IDD, pParent)
{
	TCHAR szPath[_CC_MAX_PATH_];
	m_bTipClose = FALSE;
	m_bConnecting = FALSE;
	m_cbStarting = 0;
	m_hFromWnd = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	_cc_get_module_directory(NULL, szPath, _cc_countof(szPath));

	for (INT i = 0; i < MAX_CRAWLER_DATA_BOX; i++) {
		_sntprintf_s(m_szScreenshotPathFile[i], _cc_countof(m_szScreenshotPathFile[i]), _T("%s\\data\\Screenshot-%d.jpg"), szPath, i);
	}
	_sntprintf_s(m_szPathFile,_cc_countof(m_szPathFile), _T("%s\\Crowler.cfg"),szPath);
	ReadDBConfigFile(szPath);
	_cc_init_mysql(&gSQLDriver);
}

CCrawlerDialog::~CCrawlerDialog()
{
	SaveConfigFile();
}

void CCrawlerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_HINT, m_StaticHint);
	DDX_Control(pDX, IDC_CRAWLER_LIST, m_ListCtrl);
}

BEGIN_MESSAGE_MAP(CCrawlerDialog, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()

	//菜单消息
	ON_COMMAND(IDM_SHOW,OnEventShow)
	ON_COMMAND(IDM_CLOSE,OnEventClose)
	ON_COMMAND(IDM_ABOUTBOX,OnEventAboutBox)

	ON_MESSAGE(WM_TRAY_ICON, OnMessageTrayIcon)
	ON_MESSAGE(WM_MESSAGE_EVENT, OnMessageEvent)
	//ON_STN_CLICKED(IDC_PIC_DRAG, OnStnClickedPicDrag)
	ON_BN_CLICKED(IDC_BUTTON_SETTING, &CCrawlerDialog::OnBnClickedSetting)
	ON_BN_CLICKED(IDC_BUTTON_RUNNING, &CCrawlerDialog::OnBnClickedButtonRunning)
END_MESSAGE_MAP()


// CCrawlerDialog 消息处理程序

BOOL CCrawlerDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

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
	gCrawlerScreenshot.cbSuccess = ReadConfigFile();
	
	CenterWindow();
	ShowWindow(SW_SHOWNOACTIVATE);

	m_StaticDrag.SubclassDlgItem(IDC_PIC_DRAG,this);

	// TODO: 在此添加额外的初始化代码
	// Set the ListCtrl style
	LONG lStyle = m_ListCtrl.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
	lStyle |=  LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	m_ListCtrl.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0,(LPARAM)lStyle);

	// Add ListCtrl titles
	m_ListCtrl.InsertColumn(0,_T("品种"),NULL,100, -1);
	m_ListCtrl.InsertColumn(1,_T("卖价"),NULL,120,-1);
	m_ListCtrl.InsertColumn(2,_T("买价"),NULL,120,-1);
	
	//设置托盘
	m_SystemTrayIcon.InitTrayIcon(m_hWnd, IDC_SYSTEM_TRAY_ICON);
	m_SystemTrayIcon.ShowTrayIcon(m_hIcon, gProductName);

	if (!gCrawlerScreenshot.cbSuccess) {
		INT nWidth = 0;
		for (INT i = 0; i < MAX_CRAWLER_DATA_BOX; i++) {
			gCrawlerScreenshot.nBoxWidth[i] = 100;
			nWidth += 100;
		}
		SetRect(&gCrawlerScreenshot.rcBox,0,0,nWidth,500);
		m_StaticHint.SetWindowText(_T("配置读取失败，请重新配置数据！"));
	} else {
		SetDlgItemText(IDC_EDIT_CLASS_NAME,gCrawlerScreenshot.szClassName);
		SetDlgItemText(IDC_EDIT_TITLE,gCrawlerScreenshot.szTitle);
	}

	m_bConnecting = _cc_thread_start(doWorkerInitialization, this);

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
	case EVENT_DRAG_ENDED:			//获取窗口事件
		{
			m_hFromWnd = (HWND)lParam;

			KillTimer(IDI_TIMEER_DRAG);
			if (!::IsWindowVisible(m_hFromWnd)) {
				return 0L;
			}
			::GetWindowText(m_hFromWnd,gCrawlerScreenshot.szTitle,_cc_countof(gCrawlerScreenshot.szTitle));
			//直接使用全局变量gCrawlerScreenshot.szClassName 导致无效的内存访问
			//必须使用零时变量获取类名
			TCHAR szClassName[128];
			GetClassName(m_hFromWnd,szClassName,_cc_countof(szClassName));
			_tcsncpy_s(gCrawlerScreenshot.szClassName, szClassName, _cc_countof(gCrawlerScreenshot.szClassName));
			gCrawlerScreenshot.szClassName[_cc_countof(gCrawlerScreenshot.szClassName) - 1] = 0;

			SetDlgItemText(IDC_EDIT_CLASS_NAME,gCrawlerScreenshot.szClassName);
			SetDlgItemText(IDC_EDIT_TITLE,gCrawlerScreenshot.szTitle);

			SaveConfigFile();
			return 0L;
		}
	case EVENT_SETTING_CLOSE: {
			CButton *btStarting = ((CButton*)GetDlgItem(IDC_BUTTON_RUNNING));
			if (!btStarting->IsWindowEnabled()) {
				btStarting->EnableWindow(TRUE);
			}
			SaveConfigFile();
		return 0L;
		}
	case EVENT_WORKER_INITIALIZED: {
			INT nErrorCode = (INT)lParam;
			if (nErrorCode != 0) {
				return 0L;
			}

			if (gCrawlerConfig.pProducts) {
				m_ListCtrl.DeleteAllItems();
				for (INT i = 0; i < gCrawlerConfig.nProductCount; i++) {
					TCHAR szBuf[32];
					tagProducts *product = (gCrawlerConfig.pProducts + i);
					INT nItem = m_ListCtrl.InsertItem(m_ListCtrl.GetItemCount(),product->szName);
					_sntprintf_s(szBuf, _cc_countof(szBuf), _T("%d"), product->nId);
					m_ListCtrl.SetItem(nItem,1,1,szBuf,NULL,0,0,0);
					m_ListCtrl.SetItemData(nItem, (DWORD_PTR)product);
				}
			}
			//启用
			((CButton*)GetDlgItem(IDCANCEL))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_BUTTON_SETTING))->EnableWindow(TRUE);
			m_bConnecting = FALSE;

		return 0L;
		}
	case EVENT_WORKER_CLOSEED: {

			((CButton*)GetDlgItem(IDC_BUTTON_RUNNING))->SetWindowText(_T("开始采集"));
			((CButton*)GetDlgItem(IDC_BUTTON_SETTING))->EnableWindow(TRUE);
			m_cbStarting = 0;
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
			CMenu Menu;
			Menu.CreatePopupMenu();

			//构造菜单
			Menu.AppendMenu(0,IDM_ABOUTBOX,TEXT("关于..."));
			Menu.AppendMenu(MF_SEPARATOR);
			Menu.AppendMenu(0,IDM_SHOW,TEXT("显示界面"));
			Menu.AppendMenu(0,IDM_CLOSE,TEXT("退出程序"));

			//显示菜单
			CPoint MousePoint;
			GetCursorPos(&MousePoint);
			Menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,MousePoint.x,MousePoint.y,AfxGetMainWnd());
			return 1;
		}
	}

	return 0;
}
void CCrawlerDialog::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
		OnEventAboutBox();
	} else if (nID == SC_CLOSE && m_cbStarting){
		if (!m_bTipClose) {
			MessageBox(_T("窗口隐藏在后台工作，如要退出程序请点击“退出”按钮，或者右下角图标退出程序"), gProductName, MB_OK);
			m_bTipClose = TRUE;
		}
		ShowWindow(SW_HIDE);
		return;
	}

	CDialog::OnSysCommand(nID, lParam);
}

//取消函数
void CCrawlerDialog::OnCancel(){
	if (m_bConnecting) {
		MessageBox(_T("正在初始化，请稍后再关闭。"), gProductName, MB_OK);
		return ;
	}
	if (m_cbStarting) {
		MessageBox(_T("请先停止采集，请稍后再关闭。"), gProductName, MB_OK);
		return ;
	}

	doConnectSQL();
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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

HWND hWnd = NULL;

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
	find.lpName = _T("市场报价");
	find.length = _tcslen(find.lpName);
	find.hWnd = NULL;
	hWnd = ::FindWindow(_T("MetaQuotes::MetaTrader::4.00"),NULL); 
	if (EnumChildWindow(hWnd, &find) == FALSE) {
		//MessageBox(_T("程序没有找到 ！"));
		return FALSE;
	}
	hWnd = find.hWnd;
	find.hWnd = NULL;
	;
	if (EnumChildWindow(hWnd, &find) == FALSE) {
		//MessageBox(_T("程序没有找到 ！"));
		return FALSE;
	}

	hWnd = find.hWnd;
	hWnd = ::FindWindowEx(hWnd,0,_T("SysListView32"),NULL);
	if(hWnd == NULL) {
		//MessageBox(_T("程序没有找到 ！"));
		return FALSE;
	}

	// Get the ListView lines.
	int count = (int)::SendMessage(hWnd,LVM_GETITEMCOUNT,0,0);
	int i,nItem;

	DWORD	pid;
	HANDLE	process;
	LVITEM	lvi, *_lvi;
	TCHAR	firstitem[512], secitem[512],thirditem[512],fourthitem[512];
	TCHAR	*_firstitem, *_secitem,*_thirditem,*_fourthitem;

	memset(firstitem,0,512);
	memset(secitem,0,512);
	memset(thirditem,0,512);
	memset(fourthitem,0,512);


	// Get process pid.
	GetWindowThreadProcessId(hWnd, &pid);
	process = OpenProcess(PROCESS_VM_OPERATION|PROCESS_VM_READ|
		PROCESS_VM_WRITE|PROCESS_QUERY_INFORMATION, FALSE, pid);

	_lvi = (LVITEM*)VirtualAllocEx(process, NULL, sizeof(LVITEM),MEM_COMMIT, PAGE_READWRITE);
	_firstitem = (TCHAR*)VirtualAllocEx(process, NULL, 512, MEM_COMMIT,PAGE_READWRITE);
	_secitem  = (TCHAR*)VirtualAllocEx(process, NULL, 512, MEM_COMMIT,PAGE_READWRITE);
	_thirditem = (TCHAR*)VirtualAllocEx(process, NULL, 512, MEM_COMMIT,PAGE_READWRITE);
	_fourthitem = (TCHAR*)VirtualAllocEx(process,NULL, 512, MEM_COMMIT,PAGE_READWRITE);
	lvi.cchTextMax=512;


	// Here use degression , order with the task manager
	for(i = count; i >= 0; i--) {
		// Read first item data
		lvi.iSubItem = 0;
		lvi.pszText = _firstitem;
		WriteProcessMemory(process, _lvi, &lvi, sizeof(LVITEM), NULL);
		::SendMessage(hWnd, LVM_GETITEMTEXT, (WPARAM)i, (LPARAM)_lvi);


		// Read second item data
		lvi.iSubItem = 1;
		lvi.pszText = _secitem;
		WriteProcessMemory(process, _lvi, &lvi, sizeof(LVITEM), NULL);
		::SendMessage(hWnd, LVM_GETITEMTEXT, (WPARAM)i, (LPARAM)_lvi);


		// Read 3rd item data
		lvi.iSubItem = 2;
		lvi.pszText = _thirditem;
		WriteProcessMemory(process, _lvi, &lvi, sizeof(LVITEM), NULL);
		::SendMessage(hWnd, LVM_GETITEMTEXT, (WPARAM)i, (LPARAM)_lvi);


		// Read the process memory.
		ReadProcessMemory(process, _firstitem, firstitem, 512, NULL);
		ReadProcessMemory(process, _secitem, secitem, 512, NULL);
		ReadProcessMemory(process, _thirditem, thirditem, 512, NULL);

		// Add it into my ListCtrl.
		nItem = m_ListCtrl.InsertItem(0,firstitem);
		m_ListCtrl.SetItem(nItem,1,1,secitem,NULL,0,0,0);
		m_ListCtrl.SetItem(nItem,2,1,thirditem,NULL,0,0,0);
	}


	// Release memory
	VirtualFreeEx(process, _lvi, 0, MEM_RELEASE);
	VirtualFreeEx(process, _firstitem,  0, MEM_RELEASE);
	VirtualFreeEx(process, _secitem,    0, MEM_RELEASE);
	VirtualFreeEx(process, _thirditem,  0, MEM_RELEASE);

	return TRUE;
}
*/
void CCrawlerDialog::OnBnClickedSetting()
{
	// TODO: 在此添加控件通知处理程序代码
	//GetCurrencyData();
	if (m_hFromWnd == NULL) {
		LPCTSTR lpClassName = gCrawlerScreenshot.szClassName;
		LPCTSTR lpTitle = gCrawlerScreenshot.szTitle;
		if (*lpClassName == 0) {
			lpClassName = NULL;
		}

		if (lpClassName == NULL && *lpTitle == 0) {
			MessageBox(_T("请先配置采集程序！"), gProductName, MB_OK);
			return ;
		}
		m_hFromWnd = ::FindWindow(lpClassName, lpTitle);
		if (m_hFromWnd == NULL) {
			MessageBox(_T("未找到，请先运行需要采集的程序！"), gProductName, MB_OK);
			return ;
		}
	}

	if (!::IsWindowVisible(m_hFromWnd)) {
		MessageBox(_T("需要采集的窗口不可见，请不要最小化被采集的窗口！"), gProductName, MB_OK);
		return ;
	}

	m_ScreenshotWnd.Open(m_hFromWnd);
}

void CCrawlerDialog::OnBnClickedButtonRunning()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bConnecting) {
		MessageBox(_T("正在初始化，请稍后！"), gProductName, MB_OK);
		return ;
	}

	if (m_cbStarting == 2) {
		return;
	}

	if (m_cbStarting == 1) {
		m_cbStarting = 2;
		GetDlgItem(IDC_BUTTON_RUNNING)->SetWindowText(_T("正在停止..."));
		GetDlgItem(IDC_BUTTON_RUNNING)->EnableWindow(FALSE);
	} else {
		GetDlgItem(IDC_BUTTON_RUNNING)->SetWindowText(_T("停止采集"));
		m_cbStarting = 1;
		_cc_thread_start(doStartingWorker, this);
	}
	GetDlgItem(IDC_BUTTON_SETTING)->EnableWindow(m_cbStarting == 0);

}
