#pragma once

#include "resource.h"
// CReadMemoryDialog 对话框

class CReadMemoryDialog : public CDialog
{
	DECLARE_DYNAMIC(CReadMemoryDialog)
protected:
	CListCtrl		m_ListCtrl;
	HANDLE			m_hProcess;
public:
	CReadMemoryDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CReadMemoryDialog();

// 对话框数据
	enum { IDD = IDD_READ_MEMORY_DIALOG };

public:
	void Open(DWORD dwProcessID, CWnd *pParenWnd);
	void ReadMemory(DWORD  dwBaseAddress, INT nCount);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	//创建函数
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonRead();
	//双击列表
	afx_msg void OnNMDBLClickList(NMHDR *pNMHDR, LRESULT *pResult);
};
