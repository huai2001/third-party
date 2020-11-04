// InjectDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DllInject.h"
#include "InjectDialog.h"
#include "afxdialogex.h"
#include "Hook.h"


// CInjectDialog �Ի���

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


// CInjectDialog ��Ϣ�������

BOOL CInjectDialog::OnInitDialog()
{
	__super::OnInitDialog();
	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(hIcon, FALSE);		// ����Сͼ��
	return TRUE;
}

void CInjectDialog::OnBnClickedButtonFileBrowser()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR szFilters[] = {
		_T("DLL �ļ�(*.dll)\0*.dll\0")
		_T("\0")
	};

	CFileDialog OpenDlg(TRUE);
	OpenDlg.m_ofn.lpstrTitle = _T("����ļ�");
	OpenDlg.m_ofn.lpstrFilter = szFilters;

	if (IDOK == OpenDlg.DoModal()) {
		m_EditFile.SetWindowText(OpenDlg.GetPathName());
	}
}


void CInjectDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_pProcessData) {
		TCHAR szPathName[MAX_PATH];
		m_EditFile.GetWindowText(szPathName, MAX_PATH);
		InjectDLL(m_pProcessData->dwProcessID, szPathName);
	}
	CDialog::OnOK();
}
