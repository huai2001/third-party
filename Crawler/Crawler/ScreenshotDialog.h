#pragma once


// CScreenshotDialog �Ի���

class CScreenshotDialog : public CDialog
{
	DECLARE_DYNAMIC(CScreenshotDialog)

public:
	CScreenshotDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CScreenshotDialog();

// �Ի�������
	enum { IDD = IDD_SCREENSHOT_DIALOG };
protected:
	HICON m_hIcon;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
