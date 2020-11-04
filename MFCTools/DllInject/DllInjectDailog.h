
// DllInjectDailog.h : ͷ�ļ�
//
#pragma once

#include "ProcessList.h"
#include "ProcessModuleList.h"
#include "InjectDialog.h"

// CDllInjectDailog �Ի���
class CDllInjectDailog : public CSkinDialog
{
// ����
public:
	CDllInjectDailog(CWnd* pParent = NULL);	// ��׼���캯��
public:
	CProcessList m_ProcessListCtrl;
	CProcessModuleList m_ProcessModuleListCtrl;
	CInjectDialog m_InjectDialog;
// �Ի�������
	enum { IDD = IDD_DLLINJECT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//�滭��Ϣ
	VOID OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight);
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//����б�
	afx_msg void OnRightClickProcessModuleList(NMHDR *pNMHDR, LRESULT *pResult);
	//����б�
	afx_msg void OnLeftClickProcessList(NMHDR *pNMHDR, LRESULT *pResult);
	//�һ��б�
	afx_msg void OnRightClickProcessList(NMHDR *pNMHDR, LRESULT *pResult);
};
