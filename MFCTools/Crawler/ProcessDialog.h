#pragma once

#include "ProcessList.h"
#include "ProcessModuleList.h"
// CProcessDialog 对话框

class CProcessDialog : public CSkinDialog
{
protected:
	CProcessList		m_ListCtrl;
	CProcessModuleList	m_ModuleListCtrl;
	CSkinScrollBar		m_SkinScrollBar;

public:
// 对话框数据
	enum { IDD = IDD_PROCESS_DIALOG };

	//函数定义
public:
	//构造函数
	CProcessDialog(CWnd* pParent = NULL);
	//析构函数
	virtual ~CProcessDialog();

	//重载函数
public:
	//配置消息
	virtual BOOL OnInitDialog();
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);

	DECLARE_MESSAGE_MAP()
public:
	//双击列表
	afx_msg void OnNMDBLClickList(NMHDR *pNMHDR, LRESULT *pResult);
	//点击列表
	afx_msg void OnNMLClickList(NMHDR *pNMHDR, LRESULT *pResult);
};
