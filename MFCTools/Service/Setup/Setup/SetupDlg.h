
// SetupDlg.h : ͷ�ļ�
//

#pragma once

#include <winsvc.h>

// CSetupDlg �Ի���
class CSetupDlg : public CDialogEx
{
protected:
	TCHAR m_szServicePath[MAX_PATH];
	SC_HANDLE m_hSCM;
	DWORD m_dwServiceState;
// ����
public:
	CSetupDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CSetupDlg();
// �Ի�������
	enum { IDD = IDD_SETUP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

protected:
	void Init();  
	BOOL IsInstalled();  
	BOOL Install();  
	BOOL Uninstall();
	BOOL Starting();
	BOOL Stoping();

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSelectFile();
	afx_msg void OnBnClickedRegister();
	afx_msg void OnBnClickedButtonStatus();
};
