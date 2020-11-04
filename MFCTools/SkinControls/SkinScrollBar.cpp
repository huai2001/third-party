#include "StdAfx.h"
#include "Resource.h"
#include "SkinScrollBar.h"

#ifdef _DEBUG
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
	#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////////////////
//静态变量

CSkinScrollBarAttribute			CSkinScrollBar::m_SkinAttribute;		//资源变量

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinScrollBarAttribute::CSkinScrollBarAttribute()
{

}
	
//析构函数
CSkinScrollBarAttribute::~CSkinScrollBarAttribute()
{
	//销毁资源
	if (m_ImageScroll.IsNull()==false)
	{
		m_ImageScroll.Destroy();
	}

	return;
}

//配置资源
bool CSkinScrollBarAttribute::Initialization(CSkinRenderManager * pSkinRenderManager)
{
	//加载资源
	m_ImageScroll.LoadFromResource(GetModuleHandle(SKIN_CONTROLS_DLL_NAME),IDB_SKIN_SCROLL);
	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinScrollBar::CSkinScrollBar()
{
	m_pWnd = NULL;
}

//析构函数
CSkinScrollBar::~CSkinScrollBar()
{
}

//配置滚动
VOID CSkinScrollBar::InitScroolBar(CWnd * pWnd, CBitImage &BitImage)
{
#ifndef WIN64
	//设置滚动
	SkinSB_Init(pWnd->GetSafeHwnd(),BitImage);
	m_pWnd = pWnd;
#endif
}

//配置滚动
VOID CSkinScrollBar::InitScroolBar(CWnd * pWnd)
{
#ifndef WIN64
	//设置滚动
	SkinSB_Init(pWnd->GetSafeHwnd(),m_SkinAttribute.m_ImageScroll);
	m_pWnd = pWnd;
#endif
	return;
}

void CSkinScrollBar::ShowScrollBar( BOOL bShow )
{
#ifndef WIN64
	SkinSB_ShowScrollBar(m_pWnd->GetSafeHwnd(),SB_VERT,bShow);
#endif
}
//////////////////////////////////////////////////////////////////////////////////
