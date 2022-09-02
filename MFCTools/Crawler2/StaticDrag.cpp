// StaticDrag.cpp : 实现文件
//

#include "stdafx.h"
#include "Crawler.h"
#include "StaticDrag.h"


// CStaticDrag

IMPLEMENT_DYNAMIC(CStaticDrag, CStatic)

CStaticDrag::CStaticDrag()
{
	HINSTANCE hInstance = AfxGetApp()->m_hInstance;
	m_hDragIcon[0] = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON_DRAG_1));
	m_hDragIcon[1] = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON_DRAG_2));
	m_hDragCursor = LoadCursor(hInstance,MAKEINTRESOURCE(IDC_CURSOR_DRAG));
}

CStaticDrag::~CStaticDrag()
{

}


BEGIN_MESSAGE_MAP(CStaticDrag, CStatic)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CStaticDrag 消息处理程序

void CStaticDrag::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetCapture();
	::SetCursor(m_hDragCursor);
	SetIcon(m_hDragIcon[1]);

	GetParent()->PostMessage(WM_MESSAGE_EVENT, EVENT_DRAG_STARTED, NULL);
	__super::OnLButtonDown(nFlags, point);
}

void CStaticDrag::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ReleaseCapture();
	SetIcon(m_hDragIcon[0]);
	
	GetParent()->PostMessage(WM_MESSAGE_EVENT, EVENT_DRAG_ENDED, (LPARAM)GetWindowFromPoint());
	__super::OnLButtonUp(nFlags, point);
}