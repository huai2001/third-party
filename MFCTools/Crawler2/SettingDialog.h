#pragma once

#include "resource.h"
#include "StaticColor.h"

// CSettingDialog 对话框

class CSettingDialog : public CSkinDialog
{
	DECLARE_DYNAMIC(CSettingDialog)

private:
	HWND				m_hFromWnd;
    CSkinEdit           m_EditPosX[7];
    CSkinEdit           m_EditPosY[7];
    CSkinEdit           m_EditColor[7];
    CStaticColor        m_StaticColor[7];

    CSkinButton         m_btOK;
    CSkinButton         m_btCancal;

    //函数定义
public:
    //构造函数
    CSettingDialog(CWnd* pParent = NULL);
    //析构函数
    virtual ~CSettingDialog();

// 对话框数据
	enum { IDD = IDD_SETTING_DIALOG };

    void SetFromWnd(HWND hWnd) {
        m_hFromWnd = hWnd;
    }
    //重载函数
public:
    //配置消息
    virtual BOOL OnInitDialog();
    //控件绑定
    virtual VOID DoDataExchange(CDataExchange * pDX);
    //事件消息
    afx_msg LRESULT OnMessageEvent(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
    afx_msg void OnBnClickedOk();
};
