// StaticDrag.cpp : 实现文件
//

#include "stdafx.h"
#include "Crawler.h"
#include "StaticAttach.h"


// CStaticAttach

IMPLEMENT_DYNAMIC(CStaticAttach, CStatic)

CStaticAttach::CStaticAttach()
{
	HINSTANCE hInstance = AfxGetApp()->m_hInstance;
	m_hIcon[0] = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON_ATTACH_1));
	m_hIcon[1] = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON_ATTACH_2));
}

CStaticAttach::~CStaticAttach()
{

}


BEGIN_MESSAGE_MAP(CStaticAttach, CStatic)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CStaticAttach 消息处理程序

void CStaticAttach::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetIcon(m_hIcon[1]);

	__super::OnLButtonDown(nFlags, point);
}

void CStaticAttach::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetIcon(m_hIcon[0]);
	
	GetParent()->PostMessage(WM_MESSAGE_EVENT, EVENT_OPEN_PROCESS_ATTACH, 0);
	__super::OnLButtonUp(nFlags, point);
}