#ifndef DLG_INJECT_DIALOG_HEAD_FILE
#define DLG_INJECT_DIALOG_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "ProcessList.h"

//////////////////////////////////////////////////////////////////////////////////

//注册窗口
class CInjectDialog : public CSkinDialog
{
	//变量定义
public:
	tagProcessData*					m_pProcessData;
	//配置变量
protected:

	//控件变量
public:
	CSkinEdit						m_EditFile;
	CSkinEdit						m_EditLoadFunc;
	CSkinButton						m_ButtonFileBrowser;
	CSkinButton						m_ButtonOK;
	CSkinButton						m_ButtonCancel;

	//组件变量
protected:
	CSkinLayered					m_SkinLayered;						//分层窗口

	//函数定义
public:
	//构造函数
	CInjectDialog();
	//析构函数
	virtual ~CInjectDialog();
	void SetProcessData(tagProcessData *p) {
		m_pProcessData = p;
	}
	// 对话框数据
	enum { IDD = IDD_INJECT_DAILOG };
	//重载函数
public:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//配置函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();
	//取消函数
	virtual VOID OnCancel();

	//消息映射
public:public:
	afx_msg void OnBnClickedButtonFileBrowser();
	afx_msg void OnBnClickedOk();

	//系统消息
protected:
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif