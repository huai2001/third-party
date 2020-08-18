#pragma once


// CScreenshotDialog 对话框

class CScreenshotDialog : public CDialog
{
	DECLARE_DYNAMIC(CScreenshotDialog)

public:
	CScreenshotDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CScreenshotDialog();

// 对话框数据
	enum { IDD = IDD_SCREENSHOT_DIALOG };
protected:
	HICON m_hIcon;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
