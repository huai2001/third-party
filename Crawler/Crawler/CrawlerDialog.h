
// CrawlerDialog.h : ͷ�ļ�
//
#include "ScreenshotWnd.h"
#include "StaticDrag.h"
#include "SystemTrayIcon.h"
#include "Worker.h"
#include "resource.h"
#pragma once


// CCrawlerDialog �Ի���
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
	CSystemTrayIcon	m_SystemTrayIcon; //����ͼ��
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
// ����
public:
	CCrawlerDialog(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CCrawlerDialog();

// �Ի�������
	enum { IDD = IDD_CRAWLER_DIALOG };

protected:
	//BOOL GetCurrencyData();
	BOOL ReadDBConfigFile(LPCTSTR lpPath);
	BOOL ReadConfigFile();
	BOOL SaveConfigFile();

// ʵ��
protected:
	HICON m_hIcon;

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��������
	virtual BOOL OnInitDialog();
	//ȡ������
	virtual void OnCancel();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HCURSOR OnQueryDragIcon();
	//�¼���Ϣ
	afx_msg LRESULT OnMessageEvent(WPARAM wParam, LPARAM lParam);
	//���ð�ť
	afx_msg void OnBnClickedSetting();
	//��ʾ���ڴ���
	afx_msg void OnEventAboutBox();
	//��ʾ����
	afx_msg void OnEventShow();
	//�˳�����
	afx_msg void OnEventClose();
	//��ʼ��ť
	afx_msg void OnBnClickedButtonRunning();
	//
	DECLARE_MESSAGE_MAP()
protected:
	//ͼ����Ϣ
	LRESULT OnMessageTrayIcon(WPARAM wParam, LPARAM lParam);
};
