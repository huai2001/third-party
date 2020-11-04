#include "StdAfx.h"
#include "SkinEdit.h"
#include "SkinMenu.h"
#include "SkinButton.h"
#include "SkinDialog.h"
#include "SkinSplitter.h"
#include "SkinListCtrl.h"
#include "SkinComboBox.h"
#include "SkinHyperLink.h"
#include "SkinResourceManager.h"

#ifdef _DEBUG
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
	#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////////////////
//静态变量

CSkinResourceManager *	CSkinResourceManager::m_pSkinResourceManager=NULL;

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinResourceManager::CSkinResourceManager()
{
	//设置变量
	ASSERT(m_pSkinResourceManager==NULL);
	if (m_pSkinResourceManager==NULL) m_pSkinResourceManager=this;

	return;
}

//析构函数
CSkinResourceManager::~CSkinResourceManager()
{
	//设置变量
	ASSERT(m_pSkinResourceManager==this);
	if (m_pSkinResourceManager==this) m_pSkinResourceManager=NULL;

	return;
}

//设置资源
VOID CSkinResourceManager::SetSkinResource(const tagSkinRenderInfo & SkinRenderInfo)
{
	//设置渲染
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();
	if (pSkinRenderManager!=NULL) pSkinRenderManager->SetSkinRenderInfo(SkinRenderInfo);

	//创建字体
	if (m_DefaultFont.m_hObject==NULL)
	{
		LOGFONT lf;
		memset(&lf,0,sizeof(LOGFONT));
		lf.lfWeight		= 0;				//字体磅数
		lf.lfHeight		= 12;				//字体高度(旋转后的字体宽度)  
		lf.lfWidth		= 0;				//字体宽度(旋转后的字体高度)
		lf.lfUnderline	= FALSE;			//无下划线
		lf.lfStrikeOut	= FALSE;			//无删除线
		lf.lfItalic		= FALSE;			//非斜体
		lf.lfEscapement	= 0;				//字体显示角度=270°
		lf.lfCharSet	= DEFAULT_CHARSET;	//使用缺省字符集
		lstrcpy(lf.lfFaceName,TEXT("宋体"));

		m_DefaultFont.CreateFontIndirect(&lf);
	}

	//更新资源
	CSkinScrollBar::m_SkinAttribute.Initialization(pSkinRenderManager);
	CSkinEdit::m_SkinAttribute.Initialization(pSkinRenderManager);
	CSkinComboBox::m_SkinAttribute.Initialization(pSkinRenderManager);
	CSkinListCtrl::m_SkinAttribute.Initialization(pSkinRenderManager);
	CSkinHyperLink::m_SkinAttribute.Initialization(pSkinRenderManager);
	CSkinMenuKernel::m_SkinAttribute.Initialization(pSkinRenderManager);
	CSkinDialog::m_SkinAttribute.Initialization(pSkinRenderManager);

	//更新界面
	DWORD dwThreadID=GetCurrentThreadId();
	EnumThreadWindows(dwThreadID,EnumWindowsProc,(LPARAM)pSkinRenderManager);

	return;
}

//枚举函数
BOOL CALLBACK CSkinResourceManager::EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	//枚举窗口
	EnumChildWindows(hWnd,EnumWindowsProc,lParam);

	//重画窗口
	RedrawWindow(hWnd,NULL,NULL,RDW_FRAME|RDW_ERASE|RDW_ERASENOW|RDW_UPDATENOW|RDW_INVALIDATE|RDW_NOCHILDREN);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
