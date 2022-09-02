
// SetupDlg.h : 头文件
//

#pragma once

#include <winsvc.h>

// CSetupDlg 对话框
class CSetupDlg : public CDialogEx
{
protected:
	TCHAR m_szServicePath[MAX_PATH];
	SC_HANDLE m_hSCM;
	DWORD m_dwServiceState;
// 构造
public:
	CSetupDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CSetupDlg();
// 对话框数据
	enum { IDD = IDD_SETUP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

protected:
	void Init();  
	BOOL IsInstalled();  
	BOOL Install();  
	BOOL Uninstall();
	BOOL Starting();
	BOOL Stoping();

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSelectFile();
	afx_msg void OnBnClickedRegister();
	afx_msg void OnBnClickedButtonStatus();
};
