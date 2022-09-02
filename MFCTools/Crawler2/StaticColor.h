#pragma once

class CStaticColor : public CStatic
{
    DECLARE_DYNAMIC(CStaticColor)
protected: 
    HICON m_IArrow;
    HCURSOR	m_hDragCursor;
    BOOL m_isMouseDown;

public:
    INT m_Index;
    CPoint m_MousePoint;
    COLORREF m_BackColor;

public:
    CStaticColor(void);
    virtual ~CStaticColor(void);

public:
    void SetBackColor( COLORREF BackColor );
    void SetIndex(INT index) {
        m_Index = index;
    }
protected:
    //{{AFX_MSG(CMyPic)
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    //¶¨Ê±Æ÷
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
};

