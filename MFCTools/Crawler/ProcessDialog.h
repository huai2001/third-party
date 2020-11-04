#pragma once

#include "ProcessList.h"
#include "ProcessModuleList.h"
// CProcessDialog �Ի���

class CProcessDialog : public CSkinDialog
{
protected:
	CProcessList		m_ListCtrl;
	CProcessModuleList	m_ModuleListCtrl;
	CSkinScrollBar		m_SkinScrollBar;

public:
// �Ի�������
	enum { IDD = IDD_PROCESS_DIALOG };

	//��������
public:
	//���캯��
	CProcessDialog(CWnd* pParent = NULL);
	//��������
	virtual ~CProcessDialog();

	//���غ���
public:
	//������Ϣ
	virtual BOOL OnInitDialog();
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);

	DECLARE_MESSAGE_MAP()
public:
	//˫���б�
	afx_msg void OnNMDBLClickList(NMHDR *pNMHDR, LRESULT *pResult);
	//����б�
	afx_msg void OnNMLClickList(NMHDR *pNMHDR, LRESULT *pResult);
};
