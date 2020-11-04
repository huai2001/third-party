#include "StdAfx.h"
#include "Resource.h"
#include "SkinScrollBar.h"

#ifdef _DEBUG
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
	#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////////////////
//��̬����

CSkinScrollBarAttribute			CSkinScrollBar::m_SkinAttribute;		//��Դ����

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CSkinScrollBarAttribute::CSkinScrollBarAttribute()
{

}
	
//��������
CSkinScrollBarAttribute::~CSkinScrollBarAttribute()
{
	//������Դ
	if (m_ImageScroll.IsNull()==false)
	{
		m_ImageScroll.Destroy();
	}

	return;
}

//������Դ
bool CSkinScrollBarAttribute::Initialization(CSkinRenderManager * pSkinRenderManager)
{
	//������Դ
	m_ImageScroll.LoadFromResource(GetModuleHandle(SKIN_CONTROLS_DLL_NAME),IDB_SKIN_SCROLL);
	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CSkinScrollBar::CSkinScrollBar()
{
	m_pWnd = NULL;
}

//��������
CSkinScrollBar::~CSkinScrollBar()
{
}

//���ù���
VOID CSkinScrollBar::InitScroolBar(CWnd * pWnd, CBitImage &BitImage)
{
#ifndef WIN64
	//���ù���
	SkinSB_Init(pWnd->GetSafeHwnd(),BitImage);
	m_pWnd = pWnd;
#endif
}

//���ù���
VOID CSkinScrollBar::InitScroolBar(CWnd * pWnd)
{
#ifndef WIN64
	//���ù���
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
