
// CrawlerDialog.h : 头文件
//
#include "ScreenshotWnd.h"
#include "StaticDrag.h"
#include "SystemTrayIcon.h"
#include "Worker.h"
#include "resource.h"
#pragma once


// CCrawlerDialog 对话框
class CCrawlerDialog : public CDialog
{
protected:
	BYTE			m_cbStarting;
	BOOL			m_bTipClose;
	BOOL			m_bConnecting;
	CListCtrl		m_ListCtrl;
	HWND			m_hFromWnd;
	CStatic			m_StaticHint;
	CStaticDrag		m_StaticDrag;
	CScreenshotWnd	m_ScreenshotWnd;
	CSystemTrayIcon	m_SystemTrayIcon; //任务图标
	TCHAR			m_szPathFile[_CC_MAX_PATH_];
	TCHAR			m_szScreenshotPathFile[MAX_CRAWLER_DATA_BOX][_CC_MAX_PATH_];

public:
	void SetHint(LPCTSTR lpHint) {
		if (GetSafeHwnd()) {
			m_StaticHint.SetWindowText(lpHint);
		}
	}

	BOOL IsStarting() {
		return m_cbStarting == 1;
	}

	HWND GetFromWindow() {
		return m_hFromWnd;
	}

	TCHAR* GetScreenshotPathFile(INT index){
		return m_szScreenshotPathFile[index];
	}
// 构造
public:
	CCrawlerDialog(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CCrawlerDialog();

// 对话框数据
	enum { IDD = IDD_CRAWLER_DIALOG };

protected:
	//BOOL GetCurrencyData();
	BOOL ReadDBConfigFile(LPCTSTR lpPath);
	BOOL ReadConfigFile();
	BOOL SaveConfigFile();

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

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
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
	//
	DECLARE_MESSAGE_MAP()
protected:
	//图标消息
	LRESULT OnMessageTrayIcon(WPARAM wParam, LPARAM lParam);
};
