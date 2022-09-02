#pragma once


// CStaticDrag

class CStaticDrag : public CStatic
{
	DECLARE_DYNAMIC(CStaticDrag)

public:
	CStaticDrag();
	virtual ~CStaticDrag();

public:
	HICON	m_hDragIcon[2];
	HCURSOR	m_hDragCursor;

protected:
	//{{AFX_MSG(CMyPic)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};


