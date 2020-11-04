
// DllInjectDailog.h : 头文件
//
#pragma once

#include "ProcessList.h"
#include "ProcessModuleList.h"
#include "InjectDialog.h"

// CDllInjectDailog 对话框
class CDllInjectDailog : public CSkinDialog
{
// 构造
public:
	CDllInjectDailog(CWnd* pParent = NULL);	// 标准构造函数
public:
	CProcessList m_ProcessListCtrl;
	CProcessModuleList m_ProcessModuleListCtrl;
	CInjectDialog m_InjectDialog;
// 对话框数据
	enum { IDD = IDD_DLLINJECT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	//命令函数
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//绘画消息
	VOID OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight);
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//左击列表
	afx_msg void OnRightClickProcessModuleList(NMHDR *pNMHDR, LRESULT *pResult);
	//左击列表
	afx_msg void OnLeftClickProcessList(NMHDR *pNMHDR, LRESULT *pResult);
	//右击列表
	afx_msg void OnRightClickProcessList(NMHDR *pNMHDR, LRESULT *pResult);
};
