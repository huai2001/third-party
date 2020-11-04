#pragma once

#include "resource.h"
// CReadMemoryDialog �Ի���

class CReadMemoryDialog : public CDialog
{
	DECLARE_DYNAMIC(CReadMemoryDialog)
protected:
	CListCtrl		m_ListCtrl;
	HANDLE			m_hProcess;
public:
	CReadMemoryDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CReadMemoryDialog();

// �Ի�������
	enum { IDD = IDD_READ_MEMORY_DIALOG };

public:
	void Open(DWORD dwProcessID, CWnd *pParenWnd);
	void ReadMemory(DWORD  dwBaseAddress, INT nCount);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	//��������
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonRead();
	//˫���б�
	afx_msg void OnNMDBLClickList(NMHDR *pNMHDR, LRESULT *pResult);
};
