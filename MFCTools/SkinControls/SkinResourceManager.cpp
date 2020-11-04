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
//��̬����

CSkinResourceManager *	CSkinResourceManager::m_pSkinResourceManager=NULL;

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CSkinResourceManager::CSkinResourceManager()
{
	//���ñ���
	ASSERT(m_pSkinResourceManager==NULL);
	if (m_pSkinResourceManager==NULL) m_pSkinResourceManager=this;

	return;
}

//��������
CSkinResourceManager::~CSkinResourceManager()
{
	//���ñ���
	ASSERT(m_pSkinResourceManager==this);
	if (m_pSkinResourceManager==this) m_pSkinResourceManager=NULL;

	return;
}

//������Դ
VOID CSkinResourceManager::SetSkinResource(const tagSkinRenderInfo & SkinRenderInfo)
{
	//������Ⱦ
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();
	if (pSkinRenderManager!=NULL) pSkinRenderManager->SetSkinRenderInfo(SkinRenderInfo);

	//��������
	if (m_DefaultFont.m_hObject==NULL)
	{
		LOGFONT lf;
		memset(&lf,0,sizeof(LOGFONT));
		lf.lfWeight		= 0;				//�������
		lf.lfHeight		= 12;				//����߶�(��ת���������)  
		lf.lfWidth		= 0;				//������(��ת�������߶�)
		lf.lfUnderline	= FALSE;			//���»���
		lf.lfStrikeOut	= FALSE;			//��ɾ����
		lf.lfItalic		= FALSE;			//��б��
		lf.lfEscapement	= 0;				//������ʾ�Ƕ�=270��
		lf.lfCharSet	= DEFAULT_CHARSET;	//ʹ��ȱʡ�ַ���
		lstrcpy(lf.lfFaceName,TEXT("����"));

		m_DefaultFont.CreateFontIndirect(&lf);
	}

	//������Դ
	CSkinScrollBar::m_SkinAttribute.Initialization(pSkinRenderManager);
	CSkinEdit::m_SkinAttribute.Initialization(pSkinRenderManager);
	CSkinComboBox::m_SkinAttribute.Initialization(pSkinRenderManager);
	CSkinListCtrl::m_SkinAttribute.Initialization(pSkinRenderManager);
	CSkinHyperLink::m_SkinAttribute.Initialization(pSkinRenderManager);
	CSkinMenuKernel::m_SkinAttribute.Initialization(pSkinRenderManager);
	CSkinDialog::m_SkinAttribute.Initialization(pSkinRenderManager);

	//���½���
	DWORD dwThreadID=GetCurrentThreadId();
	EnumThreadWindows(dwThreadID,EnumWindowsProc,(LPARAM)pSkinRenderManager);

	return;
}

//ö�ٺ���
BOOL CALLBACK CSkinResourceManager::EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	//ö�ٴ���
	EnumChildWindows(hWnd,EnumWindowsProc,lParam);

	//�ػ�����
	RedrawWindow(hWnd,NULL,NULL,RDW_FRAME|RDW_ERASE|RDW_ERASENOW|RDW_UPDATENOW|RDW_INVALIDATE|RDW_NOCHILDREN);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
