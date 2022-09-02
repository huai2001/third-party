#include "StdAfx.h"
#include "resource.h"
#include "StaticColor.h"

#define IDI_TIMEER_COLOR            2

IMPLEMENT_DYNAMIC(CStaticColor, CStatic)

BEGIN_MESSAGE_MAP(CStaticColor, CStatic)
    ON_WM_TIMER()
    ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

CStaticColor::CStaticColor(void)
{
    m_isMouseDown = FALSE;
    m_BackColor = RGB(255,255,255);
    HINSTANCE hInstance = AfxGetApp()->m_hInstance;
    m_IArrow = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON_ARROW));
    m_hDragCursor = LoadCursor(hInstance,MAKEINTRESOURCE(IDC_CURSOR_ARROW));
}


CStaticColor::~CStaticColor(void)
{

}

void  CStaticColor::SetBackColor( COLORREF BackColor )
{
    m_BackColor = BackColor; //ÉèÖÃ±³¾°ÑÕÉ«
}

void CStaticColor::OnPaint() 
{
    CPaintDC dc(this);
    CRect rect;
    GetClientRect( &rect );
    CBrush BGBrush;
    BGBrush.CreateSolidBrush(m_BackColor);
    dc.SelectObject(&BGBrush);
    dc.Rectangle(&rect);
    dc.SetBkMode( TRANSPARENT );
    if (m_isMouseDown == FALSE) {
        ::DrawIconEx(dc.GetSafeHdc(), (rect.Width() - 32) / 2,(rect.Height() - 32) / 2, m_IArrow, 32, 32, 0, NULL, DI_NORMAL | DI_COMPAT);
    }
    BGBrush.DeleteObject();
}

void CStaticColor::OnLButtonDown(UINT nFlags, CPoint point) 
{
    // TODO: Add your message handler code here and/or call default
    SetCapture();
    ::SetCursor(m_hDragCursor);
    m_isMouseDown = TRUE;

    CRect rc;
    GetClientRect(rc);
    InvalidateRect(rc,TRUE);
    UpdateWindow();

    SetTimer(IDI_TIMEER_COLOR, 500, NULL);
    //GetParent()->PostMessage(WM_MESSAGE_EVENT, EVENT_DRAG_STARTED, NULL);
    __super::OnLButtonDown(nFlags, point);
}

void CStaticColor::OnLButtonUp(UINT nFlags, CPoint point) 
{
    // TODO: Add your message handler code here and/or call default
    ReleaseCapture();
    m_isMouseDown = FALSE;

    CRect rc;
    GetClientRect(rc);

    KillTimer(IDI_TIMEER_COLOR);
    HWND hWnd = ::GetDesktopWindow();
    HDC hdc = ::GetDC(hWnd);
    GetCursorPos(&m_MousePoint);

    COLORREF pixel = ::GetPixel(hdc, m_MousePoint.x, m_MousePoint.y);
    if (pixel != CLR_INVALID) {
        SetBackColor(pixel);
    }

    InvalidateRect(rc,TRUE);
    UpdateWindow();
    GetParent()->PostMessage(WM_MESSAGE_EVENT, EVENT_DRAG_ENDED, (LPARAM)this);
    __super::OnLButtonUp(nFlags, point);
}


void CStaticColor::OnTimer(UINT nIDEvent) 
{
    if (IDI_TIMEER_COLOR == nIDEvent) {
        CRect rc;
        GetWindowRect(rc);
        ScreenToClient(rc);
        HWND hWnd = ::GetDesktopWindow();
        HDC hdc = ::GetDC(hWnd);
        CPoint point;
        GetCursorPos(&point);

        COLORREF pixel = ::GetPixel(hdc, point.x, point.y);
        if (pixel != CLR_INVALID) {
            int red = GetRValue(pixel);
            int green = GetGValue(pixel);
            int blue = GetBValue(pixel);
            
            SetBackColor(RGB(red,green,blue));

            InvalidateRect(rc,TRUE);
            UpdateWindow();
        }
    }

    __super::OnTimer(nIDEvent);
}