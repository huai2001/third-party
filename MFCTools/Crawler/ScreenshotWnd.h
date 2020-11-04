#pragma once

#include "Screenshots.h"



// CScreenshotWnd
class CScreenshotWnd : public CWnd
{
	DECLARE_DYNAMIC(CScreenshotWnd)

protected:
	CScreenshots	m_Screenshot;

	//ÊÇ·ñ°´ÏÂ×ó¼ü
	BOOL			m_bLButtonDown;
	CPoint			m_cpDownPoint;
	INT				m_nIndex;
	CFont			m_Font;
public:
	CScreenshotWnd();
	virtual ~CScreenshotWnd();

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);


protected:
	void DrawBox(CDC *dc);
	void UpdateBox();
	BOOL ReadConfigFile();
	BOOL SaveConfigFile();
public:
	void Open(HWND hWnd);
	//
	BOOL Screenshot(HWND hWnd);
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	afx_msg BOOL OnSetCursor(CWnd* pWnd,UINT nHitTest,UINT message);
	DECLARE_MESSAGE_MAP()
};


