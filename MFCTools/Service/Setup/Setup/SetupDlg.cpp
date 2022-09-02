
// SetupDlg.cpp : ʵ���ļ�
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

// CSetupDlg �Ի���

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


// CSetupDlg ��Ϣ�������

BOOL CSetupDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	if (IsInstalled()) {

		SetDlgItemText(IDC_REGISTER,_T("ɾ������"));
		GetDlgItem(IDC_BUTTON_STATUS)->EnableWindow(TRUE);
		if (m_dwServiceState == SERVICE_RUNNING) {
			SetDlgItemText(IDC_BUTTON_STATUS,_T("ֹͣ����"));
		} else if (m_dwServiceState == SERVICE_STOPPED) {
			SetDlgItemText(IDC_BUTTON_STATUS,_T("��������"));
		}
	}

	TCHAR szFilePath[MAX_PATH];
    ::GetModuleFileName(NULL, szFilePath, MAX_PATH);
	TCHAR *p = _tcsrchr(szFilePath,'\\');
	if (p) {
		*p = 0;
		_sntprintf_s(m_szServicePath, MAX_PATH, _T("%s\\NBService.exe"), szFilePath);;
	} else {
		SetDlgItemText(IDC_EDIT_PATH, _T("��ѡ���������ļ�"));
	}

	if (!PathFileExists(m_szServicePath)) {
		SetDlgItemText(IDC_EDIT_PATH, _T("��ѡ���������ļ�"));
	} else {
		SetDlgItemText(IDC_EDIT_PATH, m_szServicePath);
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSetupDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSetupDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CSetupDlg::Starting() {
	SERVICE_STATUS status;
    //�򿪷�����ƹ�����
    if (m_hSCM == NULL) {
        MessageBox(_T("�޷��򿪷�����ƹ�����"), SERVICE_NAME, MB_OK);
        return FALSE;
    }

    //�򿪷���
	SC_HANDLE hService = ::OpenService(m_hSCM, SERVICE_NAME, SERVICE_QUERY_STATUS|SERVICE_START|SERVICE_STOP);
    if (hService == NULL) {
		MessageBox(_T("�޷��򿪷���"), SERVICE_NAME, MB_OK);
		return FALSE;
    }

	//��������
	if (::StartService(hService, NULL, NULL) == FALSE) {
		MessageBox(_T("�޷�����ϵͳ����"),SERVICE_NAME, MB_OK);
		::CloseServiceHandle(hService);
		return FALSE;
	}

	//�ȴ��������
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
    //�򿪷�����ƹ�����
    if (m_hSCM == NULL) {
        MessageBox(_T("�޷��򿪷�����ƹ�����"), SERVICE_NAME, MB_OK);
        return FALSE;
    }

    //�򿪷���
	SC_HANDLE hService = ::OpenService(m_hSCM, SERVICE_NAME, SERVICE_QUERY_STATUS|SERVICE_START|SERVICE_STOP);
    if (hService == NULL) {
		MessageBox(_T("�޷��򿪷���"), SERVICE_NAME, MB_OK);
		return FALSE;
    }

	//ֹͣ����
	if (::ControlService(hService, SERVICE_CONTROL_STOP, &status) == FALSE) {
		MessageBox(_T("�޷�ֹͣϵͳ����"),SERVICE_NAME, MB_OK);
		::CloseServiceHandle(hService);
		return FALSE;
	}

	//�ȴ��������
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
  
    //�򿪷�����ƹ�����
	if (m_hSCM == NULL) {
		m_hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	}
    if (m_hSCM != NULL) {
        //�򿪷���
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
    //�򿪷�����ƹ�����
    if (m_hSCM == NULL) {
        MessageBox(_T("�޷��򿪷�����ƹ�����"), SERVICE_NAME, MB_OK);
        return FALSE;
    }

	if (!PathFileExists(m_szServicePath)) {
        MessageBox(_T("�޷���������"), SERVICE_NAME, MB_OK);
        return FALSE;
	}

    //��������   
    SC_HANDLE hService = ::CreateService(m_hSCM, SERVICE_NAME, SERVICE_NAME, 
		SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
		SERVICE_AUTO_START, SERVICE_ERROR_NORMAL,
		m_szServicePath, NULL, NULL, _T(""), NULL, NULL);

    if (hService == NULL) {
        MessageBox(_T("�޷���������"), SERVICE_NAME, MB_OK);
        return FALSE;
    }

    ::CloseServiceHandle(hService);
    return TRUE;
}

BOOL CSetupDlg::Uninstall() {
    if (m_hSCM == NULL) {
        MessageBox(_T("�޷��򿪷�����ƹ�����"), SERVICE_NAME, MB_OK);
        return FALSE;  
    }  
  
    SC_HANDLE hService = ::OpenService(m_hSCM, SERVICE_NAME, SERVICE_STOP | DELETE);  
  
    if (hService == NULL) {
        MessageBox(_T("�޷��򿪷���"), SERVICE_NAME, MB_OK);
        return FALSE;
    }

    SERVICE_STATUS status;
    ::ControlService(hService, SERVICE_CONTROL_STOP, &status);
  
    //ɾ������   
    BOOL bDelete = ::DeleteService(hService);  
    ::CloseServiceHandle(hService);
  
    if (bDelete) { 
        return TRUE;
	}

    return FALSE;
}

void CSetupDlg::OnBnClickedButtonSelectFile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    // Get the executable file path   
    TCHAR szFilePath[MAX_PATH];
    ::GetModuleFileName(NULL, szFilePath, MAX_PATH);
	TCHAR *p = _tcsrchr(szFilePath, '\\');
	if (p) {
		*p = 0;
	}
	const TCHAR szFilters[] = _T("exe (*.exe)|*.exe|ȫ���ļ� (*.*)|*.*||");
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (IsInstalled()) {
		if (Uninstall()) {
			SetDlgItemText(IDC_REGISTER,_T("ע�����"));
			MessageBox(_T("ɾ������ɹ�"), SERVICE_NAME, MB_OK);
			GetDlgItem(IDC_BUTTON_STATUS)->EnableWindow(FALSE);
		}
	} else {
		if (Install()) {
			SetDlgItemText(IDC_REGISTER,_T("ɾ������"));
			MessageBox(_T("ע�����ɹ�"), SERVICE_NAME, MB_OK);
			GetDlgItem(IDC_BUTTON_STATUS)->EnableWindow(TRUE);
		}
	}
}


void CSetupDlg::OnBnClickedButtonStatus()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (IsInstalled()) {
		if (m_dwServiceState == SERVICE_RUNNING) {
			if (Stoping()) {
				SetDlgItemText(IDC_BUTTON_STATUS,_T("��������"));
				MessageBox(_T("ֹͣ����ɹ�"), SERVICE_NAME, MB_OK);
			}
		} else if (m_dwServiceState == SERVICE_STOPPED) {
			if (Starting()) {
				SetDlgItemText(IDC_BUTTON_STATUS,_T("ֹͣ����"));
				MessageBox(_T("��������ɹ�"), SERVICE_NAME, MB_OK);
			}
		}
	}
}
