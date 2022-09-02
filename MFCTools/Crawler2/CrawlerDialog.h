
// CrawlerDialog.h : ͷ�ļ�
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


// CCrawlerDialog �Ի���
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

	CSystemTrayIcon		m_SystemTrayIcon; //����ͼ��

	TCHAR				m_szPathFile[_CC_MAX_PATH_];

public:
	//������ʾ
	void SetHint(LPCTSTR lpHint) {
		if (GetSafeHwnd()) {
			m_StaticHint.SetWindowText(lpHint);
		}
	}
	//�Ƿ�ʼ�ɼ�
	BOOL IsStarting() {
		return m_cbStatus == _STATUS_STARTED_;
	}
	//��ȡ���̾��
	HWND GetFromWindow() {
		return m_hFromWnd;
	}
	//��ȡ����ID
	DWORD GetProcessID() {
		return m_dwProcessID;
	}

    BOOL GetIsPlaySound() {
        return ((CButton*)GetDlgItem(IDC_CHECK_SOUND))->GetCheck() == 1;
    }
// ����
public:
	CCrawlerDialog(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CCrawlerDialog();

// �Ի�������
	enum { IDD = IDD_CRAWLER_DIALOG };

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
	//ϵͳ����
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//�滭����
	afx_msg void OnPaint();
	//��ʱ��
	afx_msg void OnTimer(UINT nIDEvent);
    //
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
    //����OnHotkey����
    afx_msg LONG OnHotKey(WPARAM wPARAM, LPARAM lPARAM);
	//
	DECLARE_MESSAGE_MAP()
protected:
	//ͼ����Ϣ
	LRESULT OnMessageTrayIcon(WPARAM wParam, LPARAM lParam);
};
