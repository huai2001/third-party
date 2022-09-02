
// SetupDlg.cpp : 实现文件
//

#include "stdafx.h"
#include <shlobj.h>
#include "Setup.h"
#include "SetupDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SERVICE_NAME "NBService"

// CSetupDlg 对话框

CSetupDlg::CSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetupDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	ZeroMemory(m_szServicePath, sizeof(m_szServicePath));
	m_hSCM = NULL;
}

CSetupDlg::~CSetupDlg() {
	if (m_hSCM) {
		::CloseServiceHandle(m_hSCM);
	}
}

void CSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSetupDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SELECT_FILE, &CSetupDlg::OnBnClickedButtonSelectFile)
	ON_BN_CLICKED(IDC_REGISTER, &CSetupDlg::OnBnClickedRegister)
	ON_BN_CLICKED(IDC_BUTTON_STATUS, &CSetupDlg::OnBnClickedButtonStatus)
END_MESSAGE_MAP()


// CSetupDlg 消息处理程序

BOOL CSetupDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	if (IsInstalled()) {

		SetDlgItemText(IDC_REGISTER,_T("删除服务"));
		GetDlgItem(IDC_BUTTON_STATUS)->EnableWindow(TRUE);
		if (m_dwServiceState == SERVICE_RUNNING) {
			SetDlgItemText(IDC_BUTTON_STATUS,_T("停止服务"));
		} else if (m_dwServiceState == SERVICE_STOPPED) {
			SetDlgItemText(IDC_BUTTON_STATUS,_T("启动服务"));
		}
	}

	TCHAR szFilePath[MAX_PATH];
    ::GetModuleFileName(NULL, szFilePath, MAX_PATH);
	TCHAR *p = _tcsrchr(szFilePath,'\\');
	if (p) {
		*p = 0;
		_sntprintf_s(m_szServicePath, MAX_PATH, _T("%s\\NBService.exe"), szFilePath);;
	} else {
		SetDlgItemText(IDC_EDIT_PATH, _T("请选择服务进程文件"));
	}

	if (!PathFileExists(m_szServicePath)) {
		SetDlgItemText(IDC_EDIT_PATH, _T("请选择服务进程文件"));
	} else {
		SetDlgItemText(IDC_EDIT_PATH, m_szServicePath);
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSetupDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSetupDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CSetupDlg::Starting() {
	SERVICE_STATUS status;
    //打开服务控制管理器
    if (m_hSCM == NULL) {
        MessageBox(_T("无法打开服务控制管理器"), SERVICE_NAME, MB_OK);
        return FALSE;
    }

    //打开服务
	SC_HANDLE hService = ::OpenService(m_hSCM, SERVICE_NAME, SERVICE_QUERY_STATUS|SERVICE_START|SERVICE_STOP);
    if (hService == NULL) {
		MessageBox(_T("无法打开服务"), SERVICE_NAME, MB_OK);
		return FALSE;
    }

	//启动服务
	if (::StartService(hService, NULL, NULL) == FALSE) {
		MessageBox(_T("无法启动系统服务"),SERVICE_NAME, MB_OK);
		::CloseServiceHandle(hService);
		return FALSE;
	}

	//等待启动完成
	while(::QueryServiceStatus(hService, &status)) {
		::Sleep(status.dwWaitHint);
		if (status.dwCurrentState == SERVICE_RUNNING) {
			m_dwServiceState = status.dwCurrentState;
            ::CloseServiceHandle(hService);
			return TRUE;
		}
	}
	::CloseServiceHandle(hService);
	return FALSE;
}

BOOL CSetupDlg::Stoping() {
	SERVICE_STATUS status;
    //打开服务控制管理器
    if (m_hSCM == NULL) {
        MessageBox(_T("无法打开服务控制管理器"), SERVICE_NAME, MB_OK);
        return FALSE;
    }

    //打开服务
	SC_HANDLE hService = ::OpenService(m_hSCM, SERVICE_NAME, SERVICE_QUERY_STATUS|SERVICE_START|SERVICE_STOP);
    if (hService == NULL) {
		MessageBox(_T("无法打开服务"), SERVICE_NAME, MB_OK);
		return FALSE;
    }

	//停止服务
	if (::ControlService(hService, SERVICE_CONTROL_STOP, &status) == FALSE) {
		MessageBox(_T("无法停止系统服务"),SERVICE_NAME, MB_OK);
		::CloseServiceHandle(hService);
		return FALSE;
	}

	//等待启动完成
	while(::QueryServiceStatus(hService, &status)) {
		::Sleep(status.dwWaitHint);
		if (status.dwCurrentState == SERVICE_STOPPED) {
			m_dwServiceState = status.dwCurrentState;
            ::CloseServiceHandle(hService);
			return TRUE;
		}
	}
	::CloseServiceHandle(hService);
	return FALSE;
}

BOOL CSetupDlg::IsInstalled() {
	BOOL bResult = FALSE;
  
    //打开服务控制管理器
	if (m_hSCM == NULL) {
		m_hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	}
    if (m_hSCM != NULL) {
        //打开服务
		SC_HANDLE hService = ::OpenService(m_hSCM, SERVICE_NAME, SERVICE_QUERY_STATUS);
        if (hService != NULL) {  
            bResult = TRUE;
			SERVICE_STATUS status;
			if (::QueryServiceStatus(hService, &status)) {
				m_dwServiceState = status.dwCurrentState;
			}
            ::CloseServiceHandle(hService);
        }
    }  
    return bResult;
}

BOOL CSetupDlg::Install() {
    //打开服务控制管理器
    if (m_hSCM == NULL) {
        MessageBox(_T("无法打开服务控制管理器"), SERVICE_NAME, MB_OK);
        return FALSE;
    }

	if (!PathFileExists(m_szServicePath)) {
        MessageBox(_T("无法创建服务"), SERVICE_NAME, MB_OK);
        return FALSE;
	}

    //创建服务   
    SC_HANDLE hService = ::CreateService(m_hSCM, SERVICE_NAME, SERVICE_NAME, 
		SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
		SERVICE_AUTO_START, SERVICE_ERROR_NORMAL,
		m_szServicePath, NULL, NULL, _T(""), NULL, NULL);

    if (hService == NULL) {
        MessageBox(_T("无法创建服务"), SERVICE_NAME, MB_OK);
        return FALSE;
    }

    ::CloseServiceHandle(hService);
    return TRUE;
}

BOOL CSetupDlg::Uninstall() {
    if (m_hSCM == NULL) {
        MessageBox(_T("无法打开服务控制管理器"), SERVICE_NAME, MB_OK);
        return FALSE;  
    }  
  
    SC_HANDLE hService = ::OpenService(m_hSCM, SERVICE_NAME, SERVICE_STOP | DELETE);  
  
    if (hService == NULL) {
        MessageBox(_T("无法打开服务"), SERVICE_NAME, MB_OK);
        return FALSE;
    }

    SERVICE_STATUS status;
    ::ControlService(hService, SERVICE_CONTROL_STOP, &status);
  
    //删除服务   
    BOOL bDelete = ::DeleteService(hService);  
    ::CloseServiceHandle(hService);
  
    if (bDelete) { 
        return TRUE;
	}

    return FALSE;
}

void CSetupDlg::OnBnClickedButtonSelectFile()
{
	// TODO: 在此添加控件通知处理程序代码
    // Get the executable file path   
    TCHAR szFilePath[MAX_PATH];
    ::GetModuleFileName(NULL, szFilePath, MAX_PATH);
	TCHAR *p = _tcsrchr(szFilePath, '\\');
	if (p) {
		*p = 0;
	}
	const TCHAR szFilters[] = _T("exe (*.exe)|*.exe|全部文件 (*.*)|*.*||");
	CFileDialog dialog(TRUE, NULL, NULL, 6, szFilters);
	
	dialog.m_ofn.lpstrInitialDir = szFilePath;
	if (dialog.DoModal() == IDOK) {
		_tcsncpy_s(m_szServicePath, dialog.GetPathName(), MAX_PATH);
		m_szServicePath[MAX_PATH - 1] = 0;

		SetDlgItemText(IDC_EDIT_PATH, m_szServicePath);
	}
}


void CSetupDlg::OnBnClickedRegister()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IsInstalled()) {
		if (Uninstall()) {
			SetDlgItemText(IDC_REGISTER,_T("注册服务"));
			MessageBox(_T("删除服务成功"), SERVICE_NAME, MB_OK);
			GetDlgItem(IDC_BUTTON_STATUS)->EnableWindow(FALSE);
		}
	} else {
		if (Install()) {
			SetDlgItemText(IDC_REGISTER,_T("删除服务"));
			MessageBox(_T("注册服务成功"), SERVICE_NAME, MB_OK);
			GetDlgItem(IDC_BUTTON_STATUS)->EnableWindow(TRUE);
		}
	}
}


void CSetupDlg::OnBnClickedButtonStatus()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IsInstalled()) {
		if (m_dwServiceState == SERVICE_RUNNING) {
			if (Stoping()) {
				SetDlgItemText(IDC_BUTTON_STATUS,_T("启动服务"));
				MessageBox(_T("停止服务成功"), SERVICE_NAME, MB_OK);
			}
		} else if (m_dwServiceState == SERVICE_STOPPED) {
			if (Starting()) {
				SetDlgItemText(IDC_BUTTON_STATUS,_T("停止服务"));
				MessageBox(_T("启动服务成功"), SERVICE_NAME, MB_OK);
			}
		}
	}
}
