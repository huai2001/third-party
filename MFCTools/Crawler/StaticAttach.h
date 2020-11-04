#pragma once


// CStaticAttach

class CStaticAttach : public CStatic
{
	DECLARE_DYNAMIC(CStaticAttach)

public:
	CStaticAttach();
	virtual ~CStaticAttach();

public:
	HICON	m_hIcon[2];
protected:
	//{{AFX_MSG(CMyPic)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};


