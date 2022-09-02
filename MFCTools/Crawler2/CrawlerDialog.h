
// CrawlerDialog.h : 头文件
//
#include "StaticDrag.h"
#include "StaticAttach.h"
#include "StaticColor.h"
#include "Worker.h"
#include "resource.h"
#pragma once

enum {
	_STATUS_STOPED_ = 0,
	_STATUS_STOPPING_,
	_STATUS_STARTED_
};


// CCrawlerDialog 对话框
class CCrawlerDialog : public CSkinDialog
{
protected:
	BYTE				m_cbStatus;
	BOOL				m_bTipClose;
	BOOL				m_bInitialized;

	HWND				m_hFromWnd;
	DWORD				m_dwProcessID;

	CStatic				m_StaticHint;
	CStaticDrag			m_StaticDrag;
	CStaticAttach		m_StaticAttach;

    CSkinEdit           m_editDelayTime;

	CSkinButton			m_btCancel;
	CSkinButton			m_btSetting;
	CSkinButton			m_btRuning;

	//CScreenshotWnd		m_ScreenshotWnd;

	CSystemTrayIcon		m_SystemTrayIcon; //任务图标

	TCHAR				m_szPathFile[_CC_MAX_PATH_];

public:
	//设置提示
	void SetHint(LPCTSTR lpHint) {
		if (GetSafeHwnd()) {
			m_StaticHint.SetWindowText(lpHint);
		}
	}
	//是否开始采集
	BOOL IsStarting() {
		return m_cbStatus == _STATUS_STARTED_;
	}
	//获取进程句柄
	HWND GetFromWindow() {
		return m_hFromWnd;
	}
	//获取进程ID
	DWORD GetProcessID() {
		return m_dwProcessID;
	}

    BOOL GetIsPlaySound() {
        return ((CButton*)GetDlgItem(IDC_CHECK_SOUND))->GetCheck() == 1;
    }
// 构造
public:
	CCrawlerDialog(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CCrawlerDialog();

// 对话框数据
	enum { IDD = IDD_CRAWLER_DIALOG };

// 实现
protected:
	HICON m_hIcon;

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//创建函数
	virtual BOOL OnInitDialog();
	//取消函数
	virtual void OnCancel();
	//系统命令
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//绘画界面
	afx_msg void OnPaint();
	//定时器
	afx_msg void OnTimer(UINT nIDEvent);
    //
	afx_msg HCURSOR OnQueryDragIcon();
	//事件消息
	afx_msg LRESULT OnMessageEvent(WPARAM wParam, LPARAM lParam);
	//设置按钮
	afx_msg void OnBnClickedSetting();
	//显示关于窗口
	afx_msg void OnEventAboutBox();
	//显示窗口
	afx_msg void OnEventShow();
	//退出程序
	afx_msg void OnEventClose();
	//开始按钮
	afx_msg void OnBnClickedButtonRunning();
    //声明OnHotkey方法
    afx_msg LONG OnHotKey(WPARAM wPARAM, LPARAM lPARAM);
	//
	DECLARE_MESSAGE_MAP()
protected:
	//图标消息
	LRESULT OnMessageTrayIcon(WPARAM wParam, LPARAM lParam);
};
