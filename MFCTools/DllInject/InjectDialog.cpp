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
	//��ť��Ϣ
	ON_BN_CLICKED(IDC_BUTTON_FILE_BROWSER, &CInjectDialog::OnBnClickedButtonFileBrowser)
	ON_WM_SIZE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CInjectDialog::CInjectDialog() : CSkinDialog(CInjectDialog::IDD)
{
	//���ñ���
	m_pProcessData = NULL;

	return;
}

//��������
CInjectDialog::~CInjectDialog()
{
}

//�ؼ���
VOID CInjectDialog::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);

	//��ť�ؼ�

	DDX_Control(pDX, IDC_EDIT_FILE, m_EditFile);
	DDX_Control(pDX, IDC_EDIT_LOAD_FUNC, m_EditLoadFunc);
	DDX_Control(pDX, IDC_BUTTON_FILE_BROWSER, m_ButtonFileBrowser);
	DDX_Control(pDX, IDOK, m_ButtonOK);
	DDX_Control(pDX, IDCANCEL, m_ButtonCancel);
}

//��Ϣ����
BOOL CInjectDialog::PreTranslateMessage(MSG * pMsg)
{
	return __super::PreTranslateMessage(pMsg);
}

//���ú���
BOOL CInjectDialog::OnInitDialog()
{
	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(hIcon, TRUE);		// ���ô�ͼ��
	SetIcon(hIcon, FALSE);		// ����Сͼ��

	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("ע�� DLL"));

	return FALSE;
}

//ȷ������
VOID CInjectDialog::OnOK()
{
	//���ش���

	if (m_pProcessData) {
		TCHAR szPathName[MAX_PATH];
		m_EditFile.GetWindowText(szPathName, MAX_PATH);
		if (InjectDLL(m_pProcessData->dwProcessID, szPathName)) {
			AfxMessageBox(_T("ע��ɹ�"));
		}
	}

	ShowWindow(SW_HIDE);
	return;
}

//ȡ������
VOID CInjectDialog::OnCancel()
{
	//���ش���
	ShowWindow(SW_HIDE);

	return;
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
