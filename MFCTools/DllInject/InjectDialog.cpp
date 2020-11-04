#include "StdAfx.h"
#include "Hook.h"
#include "InjectDialog.h"

#ifdef _DEBUG
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
	#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CInjectDialog, CSkinDialog)
	//按钮消息
	ON_BN_CLICKED(IDC_BUTTON_FILE_BROWSER, &CInjectDialog::OnBnClickedButtonFileBrowser)
	ON_WM_SIZE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CInjectDialog::CInjectDialog() : CSkinDialog(CInjectDialog::IDD)
{
	//设置变量
	m_pProcessData = NULL;

	return;
}

//析构函数
CInjectDialog::~CInjectDialog()
{
}

//控件绑定
VOID CInjectDialog::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//按钮控件

	DDX_Control(pDX, IDC_EDIT_FILE, m_EditFile);
	DDX_Control(pDX, IDC_EDIT_LOAD_FUNC, m_EditLoadFunc);
	DDX_Control(pDX, IDC_BUTTON_FILE_BROWSER, m_ButtonFileBrowser);
	DDX_Control(pDX, IDOK, m_ButtonOK);
	DDX_Control(pDX, IDCANCEL, m_ButtonCancel);
}

//消息解释
BOOL CInjectDialog::PreTranslateMessage(MSG * pMsg)
{
	return __super::PreTranslateMessage(pMsg);
}

//配置函数
BOOL CInjectDialog::OnInitDialog()
{
	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(hIcon, TRUE);		// 设置大图标
	SetIcon(hIcon, FALSE);		// 设置小图标

	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("注入 DLL"));

	return FALSE;
}

//确定函数
VOID CInjectDialog::OnOK()
{
	//隐藏窗口

	if (m_pProcessData) {
		TCHAR szPathName[MAX_PATH];
		m_EditFile.GetWindowText(szPathName, MAX_PATH);
		if (InjectDLL(m_pProcessData->dwProcessID, szPathName)) {
			AfxMessageBox(_T("注入成功"));
		}
	}

	ShowWindow(SW_HIDE);
	return;
}

//取消函数
VOID CInjectDialog::OnCancel()
{
	//隐藏窗口
	ShowWindow(SW_HIDE);

	return;
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
