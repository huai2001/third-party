// InjectDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "DllInject.h"
#include "InjectDialog.h"
#include "afxdialogex.h"
#include "Hook.h"


// CInjectDialog 对话框

IMPLEMENT_DYNAMIC(CInjectDialog, CDialog)

CInjectDialog::CInjectDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CInjectDialog::IDD, pParent)
{
	m_pProcessData = NULL;
}

CInjectDialog::~CInjectDialog()
{
}

void CInjectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_FILE, m_EditFile);
	DDX_Control(pDX, IDC_EDIT_LOAD_FUNC, m_EditLoadFunc);
	DDX_Control(pDX, IDC_EDIT_UNLOAD_FUNC, m_EditUnloadFunc);
	DDX_Control(pDX, IDC_BUTTON_FILE_BROWSER, m_ButtonFileBrowser);
	DDX_Control(pDX, IDOK, m_ButtonOK);
	DDX_Control(pDX, IDCANCEL, m_ButtonCancel);
}


BEGIN_MESSAGE_MAP(CInjectDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_FILE_BROWSER, &CInjectDialog::OnBnClickedButtonFileBrowser)
	ON_BN_CLICKED(IDOK, &CInjectDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CInjectDialog 消息处理程序

BOOL CInjectDialog::OnInitDialog()
{
	__super::OnInitDialog();
	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(hIcon, TRUE);			// 设置大图标
	SetIcon(hIcon, FALSE);		// 设置小图标
	return TRUE;
}

void CInjectDialog::OnBnClickedButtonFileBrowser()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR szFilters[] = {
		_T("DLL 文件(*.dll)\0*.dll\0")
		_T("\0")
	};

	CFileDialog OpenDlg(TRUE);
	OpenDlg.m_ofn.lpstrTitle = _T("浏览文件");
	OpenDlg.m_ofn.lpstrFilter = szFilters;

	if (IDOK == OpenDlg.DoModal()) {
		m_EditFile.SetWindowText(OpenDlg.GetPathName());
	}
}


void CInjectDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pProcessData) {
		TCHAR szPathName[MAX_PATH];
		m_EditFile.GetWindowText(szPathName, MAX_PATH);
		InjectDLL(m_pProcessData->dwProcessID, szPathName);
	}
	CDialog::OnOK();
}
