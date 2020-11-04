#pragma once
#include "ProcessList.h"

// CInjectDialog 对话框

class CInjectDialog : public CDialog
{
	DECLARE_DYNAMIC(CInjectDialog)
public:
	CSkinEdit m_EditFile;
	CSkinEdit m_EditLoadFunc;
	CSkinEdit m_EditUnloadFunc;
	CSkinButton m_ButtonFileBrowser;
	CSkinButton m_ButtonOK;
	CSkinButton m_ButtonCancel;

	tagProcessData*	m_pProcessData;

public:
	CInjectDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInjectDialog();


	void SetProcessData(tagProcessData *p) {
		m_pProcessData = p;
	}

// 对话框数据
	enum { IDD = IDD_INJECT_DAILOG };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonFileBrowser();
	afx_msg void OnBnClickedOk();
};
