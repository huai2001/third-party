#include "StdAfx.h"
#include "Resource.h"
#include "SkinMenu.h"

#ifdef _DEBUG
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
	#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////////////////
//��������

//��϶����
#define SPACE_LEFT					3									//��϶��С
#define SPACE_RIGHT					3									//��϶��С

//��С����
#define MENU_BAR_CX					22									//����߶�
#define MENU_ITEM_CY				22									//����߶�
#define SEPARATOR_CY				3									//��ָ߶�

//////////////////////////////////////////////////////////////////////////////////

//�˵�����
CMenuImageArray						CSkinMenu::m_MenuItemImage;			//ͼ������
CMenuStringArray					CSkinMenu::m_MenuItemString;		//�ַ�����
CMenuSeparatorArray					CSkinMenu::m_MenuItemSeparator;		//�������

//�ں˱���
CSkinMenuAttribute					CSkinMenuKernel::m_SkinAttribute;	//�˵�����


static LRESULT WINAPI CallWndProc(int, WPARAM, LPARAM); // ��װ�Ĺ��ӵĴ��ڹ���
static LRESULT WINAPI MenuWndProc(HWND, UINT, WPARAM, LPARAM); // ��������˵��Ĵ��ڹ���
static WNDPROC oldWndProc = NULL; // �������汻�滻�Ĵ��ڹ���

// �����Ҫȥ���˵����ⲿ�߿���Ҫͨ����װ���ӣ�����������Բ��ı�˵���С
LRESULT WINAPI CallWndProc(int code, WPARAM wParam, LPARAM lParam)
{
	CWPSTRUCT* pStruct = (CWPSTRUCT*)lParam;
	//HC_ACTION Ϊ���봦��
	while (code == HC_ACTION) {
		HWND hWnd = pStruct->hwnd;
		// ��׽������ϢWM_CREATE������ɸѡΪ�Ƿ��ǲ˵��Ĵ���
		if ( pStruct->message != WM_CREATE)
			break;

		TCHAR sClassName[10];
		int Count = ::GetClassName(hWnd, sClassName, sizeof(sClassName)/sizeof(sClassName[0]));
		// ����Ƿ�˵����ڣ�#32768Ϊ�˵�����
		if ( Count != 6 || _tcscmp(sClassName, _T("#32768")) != 0 ) 
			break;
		//���ָ�����ڵ���Ϣ GWL_WNDPROC�õ����ڻص������ĵ�ַ����
		WNDPROC lastWndProc = (WNDPROC)GetWindowLong(hWnd, GWL_WNDPROC);
		//��ȡ��������д��ڵĴ��ڹ��̣�������ڹ������ڽ��պʹ���ϵͳ�򴰿ڷ��͵���Ϣ
		if (lastWndProc != MenuWndProc) { 
			// �滻�˵����ڹ��� 
			SetWindowLong(hWnd, GWL_WNDPROC, (long)MenuWndProc); 
			// ����ԭ�еĴ��ڹ��� 
			oldWndProc = lastWndProc; 
		}
		break; 
	}

	//ÿһ�����Ӻ����ڽ��д���ʱ��Ҫ�����Ƿ���Ҫ���¼����ݸ���һ�����Ӵ������������Ҫ���ݣ���Ҫ���ú���CallNestHookEx������
	//��ʵ��ʹ��ʱ����ǿ�ҽ��������Ƿ���Ҫ�����¼����ݶ�Ҫ�ڹ��̵�������һ��CallNextHookEx( )�����򽫻�����һЩ�޷�Ԥ֪��ϵͳ��Ϊ����ϵͳ������
	return CallNextHookEx((HHOOK)WH_CALLWNDPROC, code, wParam, lParam); 
}

// ����˵��Ĵ��ڹ���
LRESULT WINAPI MenuWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult;
	switch (message) 
	{ 
	case WM_CREATE://�Ƿ��ǲ˵��Ĵ���
		{ 
			// ����Ҫȥ���˵����ڵ�һЩ��չ���
			// ������WS_BORDER��WS_EX_DLGMODALFRAME��WS_EX_WINDOWEDGE
			lResult = CallWindowProc(oldWndProc, hWnd, message, wParam, lParam); 
			DWORD dwStyle = ::GetWindowLong(hWnd, GWL_STYLE); 
			DWORD dwNewStyle = (dwStyle & ~WS_BORDER);

			::SetWindowLong(hWnd, GWL_STYLE, dwNewStyle); 
			DWORD dwExStyle = ::GetWindowLong(hWnd, GWL_EXSTYLE);
			DWORD dwNewExStyle = (dwExStyle & ~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE)); 
			::SetWindowLong(hWnd, GWL_EXSTYLE, dwNewExStyle); 
			return lResult; 
		} 
	case WM_PRINT: // �˴���ֹ�ǿͻ����ػ���
		{
			return CallWindowProc( oldWndProc, hWnd, WM_PRINTCLIENT, wParam, lParam);
		}
	case WM_WINDOWPOSCHANGING: 
		{ 
			// ���,����������MeasureItem��ָ���˲˵���С����ϵͳ���Զ���˵��ӱ߿�
			// ��˱���ȥ���˲��ֶ���سߴ磬���˵���С��С
			LPWINDOWPOS lpPos = (LPWINDOWPOS)lParam; 
			lpPos->cx -= 2 * GetSystemMetrics(SM_CXBORDER) + 4; 
			lpPos->cy -= 2 * GetSystemMetrics(SM_CYBORDER) + 4;
			lResult = CallWindowProc(oldWndProc, hWnd, message, wParam, lParam); 
			return 0;
		} 
	case WM_GETICON:
		return 0; 
	default: 
		return CallWindowProc( oldWndProc, hWnd, message, wParam, lParam); 
	} 
}
//////////////////////////////////////////////////////////////////////////////////

//���캯��
CSkinMenuAttribute::CSkinMenuAttribute()
{
	//������ɫ
	m_crNormalText=RGB(0,0,0);	
	m_crSelectText=RGB(0,0,0);	

	//�߿���ɫ
	m_crMenuBar=RGB(0,0,0);	
	m_crSeparator=RGB(0,0,0);	
	m_crNormalBack=RGB(0,0,0);
	m_crSelectBack=RGB(0,0,0);	
	m_crSelectBorder=RGB(0,0,0);

	return;
}

//��������
CSkinMenuAttribute::~CSkinMenuAttribute() 
{
}

//������Դ
bool CSkinMenuAttribute::Initialization(CSkinRenderManager * pSkinRenderManager)
{
	//������ɫ
	m_crNormalText = RGB(0,96,159);
	m_crSelectText = RGB(0,96,159);

	//
	//
	//
	//�߿���ɫ
	m_crMenuBar=RGB(108,206,255);//pSkinRenderManager->RenderColor(0.70,0.70);
	m_crSeparator=RGB(108,206,255);//pSkinRenderManager->RenderColor(0.95,0.90);
	m_crNormalBack=pSkinRenderManager->RenderColor(0.00,1.00);
	m_crSelectBack=RGB(229,249,255);;//pSkinRenderManager->RenderColor(0.13,0.98);
	m_crSelectBorder=RGB(108,206,255);//pSkinRenderManager->RenderColor(0.66,0.87);

	return true;
}
//////////////////////////////////////////////////////////////////////////////////
volatile long CSkinMenuKernel::m_HookRefcount = 0;
HHOOK CSkinMenuKernel::m_Hook = NULL;
//���캯��
CSkinMenuKernel::CSkinMenuKernel()
{
	if (InterlockedExchangeAdd(&CSkinMenuKernel::m_HookRefcount,1) == 0) {
		// ��ȡ��ǰ�̵߳�ID
		DWORD id = ::GetCurrentThreadId();
		HINSTANCE hResInstance = GetModuleHandle(SKIN_CONTROLS_DLL_NAME);
		CSkinMenuKernel::m_Hook = SetWindowsHookEx(WH_CALLWNDPROC,CallWndProc,hResInstance,id);
	}
}

//��������
CSkinMenuKernel::~CSkinMenuKernel()
{
	if (CSkinMenuKernel::m_Hook && InterlockedExchangeAdd(&CSkinMenuKernel::m_HookRefcount,-1) == 1) {
		UnhookWindowsHookEx(CSkinMenuKernel::m_Hook);
		CSkinMenuKernel::m_Hook = NULL;
	}
}

//�滭����
VOID CSkinMenuKernel::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//Ч�����
	ASSERT(lpDrawItemStruct->CtlType==ODT_MENU);
	if (lpDrawItemStruct->CtlType!=ODT_MENU) return;

	//��������
	CRect rcItem = lpDrawItemStruct->rcItem;
	CDC * pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CSkinMenuItem * pSkinMenuItem = (CSkinMenuItem *)lpDrawItemStruct->itemData;

	//״̬����
	bool bChecked=((lpDrawItemStruct->itemState&ODS_CHECKED)!=0);
	bool bSelected=((lpDrawItemStruct->itemState&ODS_SELECTED)!=0);
	bool bGrayed=((lpDrawItemStruct->itemState&ODS_DISABLED)||(lpDrawItemStruct->itemState&ODS_GRAYED));

	//�滭����
	COLORREF crMenuBar = m_SkinAttribute.m_crMenuBar;
	COLORREF crBorder = m_SkinAttribute.m_crSelectBorder;
	COLORREF crSelectBack = m_SkinAttribute.m_crSelectBack;
	COLORREF crNormalBack = m_SkinAttribute.m_crNormalBack;

	pDC->FillSolidRect(0,rcItem.top,MENU_BAR_CX,rcItem.Height(),crMenuBar);
	pDC->FillSolidRect(rcItem.left + MENU_BAR_CX,rcItem.top,rcItem.Width() - MENU_BAR_CX,rcItem.Height(),crNormalBack);

	//����߿�
	if ((bSelected==true)&&(bGrayed==false)&&(pSkinMenuItem!=NULL)&&(pSkinMenuItem->m_MenuItemType!=MenuItemType_Separator))
	{
		pDC->Draw3dRect(rcItem.left+1,rcItem.top+1,rcItem.Width()-2,rcItem.Height()-2,crBorder,crBorder);
		pDC->FillSolidRect(rcItem.left+2,rcItem.top+2,rcItem.Width()-4,rcItem.Height()-4,crSelectBack);
	}

	//ѡ���־
	if (bChecked==true)
	{
		//������Դ
		if (m_ImageMenuFlags.IsNull()) {
			m_ImageMenuFlags.LoadFromResource(GetModuleHandle(SKIN_CONTROLS_DLL_NAME),IDB_SKIN_MENU_FLAGS);
		}

		//��Ⱦ��Դ
		CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();
		if (pSkinRenderManager!=NULL) pSkinRenderManager->RenderImage(m_ImageMenuFlags);

		//�滭����
		m_ImageMenuFlags.BitBlt(pDC->m_hDC,rcItem.left+1,rcItem.top+1,m_ImageMenuFlags.GetWidth()/2,
			m_ImageMenuFlags.GetHeight(),((bSelected==true)&&(bGrayed==false))?m_ImageMenuFlags.GetWidth()/2:0,0);
	}

	//�����˵�
	if (pSkinMenuItem == NULL)
	{
		//��ȡ��Ϣ
		MENUITEMINFO MenuItemInfo;
		MenuItemInfo.cbSize=sizeof(MenuItemInfo);
		MenuItemInfo.fMask=MIIM_FTYPE|MIIM_BITMAP;
		GetMenuItemInfo(lpDrawItemStruct->itemID,&MenuItemInfo);

		return;
	}

	//����˵�
	switch (pSkinMenuItem->m_MenuItemType)
	{
	case MenuItemType_Image:		//ͼ�β˵�
		{
			//��������
			CSkinMenuImage * pSkinMenuImage=(CSkinMenuImage *)pSkinMenuItem;

			//���� DC
			CDC ImageDC;
			ImageDC.CreateCompatibleDC(pDC);
			ImageDC.SelectObject(pSkinMenuImage->m_hBitmap);

			//��ȡ��Ϣ
			BITMAP BitmapInfo;
			GetObject(pSkinMenuImage->m_hBitmap,sizeof(BitmapInfo),&BitmapInfo);

			//�滭λͼ
			INT nYPos=rcItem.top+3;
			INT nXPos=rcItem.left+MENU_BAR_CX+SPACE_LEFT;
			pDC->BitBlt(nXPos,nYPos,BitmapInfo.bmWidth,BitmapInfo.bmHeight,&ImageDC,0,0,SRCCOPY);

			//�ͷ� DC
			ImageDC.DeleteDC();

			break;
		}
	case MenuItemType_String:		//�ַ��˵�
		{
			//��������
			CSkinMenuString * pSkinMenuString=(CSkinMenuString *)pSkinMenuItem;

			//������ɫ
			pDC->SetBkMode(TRANSPARENT);
			pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

			//������ɫ
			if (bGrayed==true) pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
			else if (bSelected==true) pDC->SetTextColor(m_SkinAttribute.m_crSelectText);
			else pDC->SetTextColor(m_SkinAttribute.m_crNormalText);

			//�滭�ַ�
			CRect rcString;
			rcString.top=rcItem.top;
			rcString.bottom=rcItem.bottom;
			rcString.right=rcItem.right-SPACE_RIGHT;
			rcString.left=rcItem.left+MENU_BAR_CX+SPACE_LEFT;
			pDC->DrawText(pSkinMenuString->m_strString,pSkinMenuString->m_strString.GetLength(),&rcString,DT_SINGLELINE|DT_VCENTER|DT_NOCLIP);

			break;
		}
	case MenuItemType_Separator:	//��ֲ˵�
		{
			//�滭���
			pDC->FillSolidRect(rcItem.left+MENU_BAR_CX+SPACE_LEFT,rcItem.top+1,rcItem.Width()-MENU_BAR_CX-SPACE_LEFT-SPACE_RIGHT,
				rcItem.Height()-2,m_SkinAttribute.m_crSeparator);

			break;
		}
	}

	return;
}

//����λ��
VOID CSkinMenuKernel::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	//Ч�����
	ASSERT(lpMeasureItemStruct->CtlType==ODT_MENU);
	if (lpMeasureItemStruct->CtlType!=ODT_MENU) return;

	//��ȡ����
	CSkinMenuItem * pSkinMenuItem=(CSkinMenuItem *)lpMeasureItemStruct->itemData;
	if (pSkinMenuItem==NULL)
	{
		//��ȡ��Ϣ
		MENUITEMINFO MenuItemInfo;
		MenuItemInfo.cbSize=sizeof(MenuItemInfo);
		MenuItemInfo.fMask=MIIM_FTYPE|MIIM_STRING;
		GetMenuItemInfo(lpMeasureItemStruct->itemID,&MenuItemInfo);

		return;
	}

	//����˵�
	switch (pSkinMenuItem->m_MenuItemType)
	{
	case MenuItemType_Image:		//ͼ�β˵�
		{
			//��������
			CSkinMenuImage * pSkinMenuImage=(CSkinMenuImage *)pSkinMenuItem;

			//��ȡ��Ϣ
			BITMAP BitmapInfo;
			GetObject(pSkinMenuImage->m_hBitmap,sizeof(BitmapInfo),&BitmapInfo);

			//�����С
			lpMeasureItemStruct->itemHeight=BitmapInfo.bmHeight+6;
			lpMeasureItemStruct->itemWidth=BitmapInfo.bmWidth+MENU_BAR_CX+SPACE_LEFT+SPACE_RIGHT;

			break;
		}
	case MenuItemType_String:		//�ַ��˵�
		{
			//��������
			CSkinMenuString * pSkinMenuString=(CSkinMenuString *)pSkinMenuItem;

			//���� DC
			HDC hDC=GetDC(NULL);
			SelectObject(hDC,CSkinResourceManager::GetInstance()->GetDefaultFont());

			//���㳤��
			CSize SizeString;
			GetTextExtentPoint32(hDC,pSkinMenuString->m_strString,pSkinMenuString->m_strString.GetLength(),&SizeString);

			//����λ��
			lpMeasureItemStruct->itemHeight=MENU_ITEM_CY;
			lpMeasureItemStruct->itemWidth=SizeString.cx+MENU_BAR_CX+SPACE_LEFT+SPACE_RIGHT;

			//�ͷ���Դ
			ReleaseDC(NULL,hDC);

			break;
		}
	case MenuItemType_Separator:	//��ֲ˵�
		{
			lpMeasureItemStruct->itemWidth=0;
			lpMeasureItemStruct->itemHeight=SEPARATOR_CY;

			break;
		}
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CSkinMenu::CSkinMenu()
{
}

//��������
CSkinMenu::~CSkinMenu()
{
	DestroyMenu();
}

//�����˵�
bool CSkinMenu::CreateMenu()
{
	return m_SkinMenuKernel.CreatePopupMenu()?true:false;
}

//���ٲ˵�
bool CSkinMenu::DestroyMenu()
{
	//���ٲ˵�
	m_SkinMenuKernel.DestroyMenu();

	//�ͷ�����
	for (INT_PTR i=0;i<m_MenuItemActive.GetCount();i++)
	{
		FreeMenuItem(m_MenuItemActive[i]);
	}
	m_MenuItemActive.RemoveAll();

	return true;
}

//�����˵�
bool CSkinMenu::TrackPopupMenu(CWnd * pWnd)
{
	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);

	//�����˵�
	m_SkinMenuKernel.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,MousePoint.x,MousePoint.y,pWnd);

	return true;
}

//�����˵�
bool CSkinMenu::TrackPopupMenu(INT nXPos, INT nYPos, CWnd * pWnd)
{
	//�����˵�
	m_SkinMenuKernel.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,nXPos,nYPos,pWnd,NULL)?true:false;

	return true;
}

//������
bool CSkinMenu::AppendSeparator()
{
	//��ȡ����
	CSkinMenuSeparator * pSkinMenuItem=(CSkinMenuSeparator *)AcitveMenuItem(MenuItemType_Separator);

	//����˵�
	if (pSkinMenuItem!=NULL)
	{
		m_SkinMenuKernel.AppendMenu(MF_OWNERDRAW,0,(LPCTSTR)(pSkinMenuItem));
		return true;
	}

	return false;
}

//����λͼ
bool CSkinMenu::AppendMenu(UINT nMenuID, HBITMAP hBitmap, UINT nFlags)
{
	//�����ж�
	ASSERT((nFlags&MF_SEPARATOR)==0);
	if ((nFlags&MF_SEPARATOR)!=0) return false;

	//��ȡ����
	CSkinMenuImage * pSkinMenuItem=(CSkinMenuImage *)AcitveMenuItem(MenuItemType_Image);

	//����˵�
	if (pSkinMenuItem!=NULL)
	{
		pSkinMenuItem->m_hBitmap=hBitmap;
		m_SkinMenuKernel.AppendMenu(MF_OWNERDRAW|nFlags,nMenuID,(LPCTSTR)(pSkinMenuItem));
		return true;
	}

	return false;
}

//�����ַ�
bool CSkinMenu::AppendMenu(UINT nMenuID, LPCTSTR pszString, UINT nFlags)
{
	//�����ж�
	ASSERT((nFlags&MF_SEPARATOR)==0);
	if ((nFlags&MF_SEPARATOR)!=0) return false;

	//��ȡ����
	CSkinMenuString * pSkinMenuItem=(CSkinMenuString *)AcitveMenuItem(MenuItemType_String);

	//����˵�
	if (pSkinMenuItem!=NULL)
	{
		pSkinMenuItem->m_strString=pszString;
		m_SkinMenuKernel.AppendMenu(MF_OWNERDRAW|nFlags,nMenuID,(LPCTSTR)(pSkinMenuItem));
		return true;
	}

	return false;
}

//������
bool CSkinMenu::InsertSeparator(UINT nPosition)
{
	//��ȡ����
	CSkinMenuSeparator * pSkinMenuItem=(CSkinMenuSeparator *)AcitveMenuItem(MenuItemType_Separator);

	//����˵�
	if (pSkinMenuItem!=NULL)
	{
		m_SkinMenuKernel.InsertMenu(nPosition,MF_OWNERDRAW,0,(LPCTSTR)(pSkinMenuItem));
		return true;
	}

	return false;
}

//����λͼ
bool CSkinMenu::InsertMenu(UINT nMenuID, HBITMAP hBitmap, UINT nPosition, UINT nFlags)
{
	//�����ж�
	ASSERT((nFlags&MF_SEPARATOR)==0);
	if ((nFlags&MF_SEPARATOR)!=0) return false;

	//��ȡ����
	CSkinMenuImage * pSkinMenuItem=(CSkinMenuImage *)AcitveMenuItem(MenuItemType_Image);

	//����˵�
	if (pSkinMenuItem!=NULL)
	{
		pSkinMenuItem->m_hBitmap=hBitmap;
		m_SkinMenuKernel.InsertMenu(nPosition,MF_OWNERDRAW|nFlags,nMenuID,(LPCTSTR)(pSkinMenuItem));
		return true;
	}

	return false;
}

//�����ַ�
bool CSkinMenu::InsertMenu(UINT nMenuID, LPCTSTR pszString, UINT nPosition, UINT nFlags)
{
	//�����ж�
	ASSERT((nFlags&MF_SEPARATOR)==0);
	if ((nFlags&MF_SEPARATOR)!=0) return false;

	//��ȡ����
	CSkinMenuString * pSkinMenuItem=(CSkinMenuString *)AcitveMenuItem(MenuItemType_String);

	//����˵�
	if (pSkinMenuItem!=NULL)
	{
		pSkinMenuItem->m_strString=pszString;
		m_SkinMenuKernel.InsertMenu(nPosition,MF_OWNERDRAW|nFlags,nMenuID,(LPCTSTR)(pSkinMenuItem));
		return true;
	}

	return false;
}

//ɾ���˵�
bool CSkinMenu::RemoveMenu(UINT nPosition, UINT nFlags)
{
	return m_SkinMenuKernel.RemoveMenu(nPosition,nFlags)?true:false;
}

//�޸Ĳ˵�
bool CSkinMenu::ModifyMenu(UINT nMenuID, HBITMAP hBitmap, UINT nPosition, UINT nFlags)
{
	return 0;
}

//�޸Ĳ˵�
bool CSkinMenu::ModifyMenu(UINT nMenuID, LPCTSTR pszString, UINT nPosition, UINT nFlags)
{
	return 0;
}

//����Ĭ��
bool CSkinMenu::SetDefaultItem(UINT uItem, BOOL fByPos)
{
	return m_SkinMenuKernel.SetDefaultItem(uItem,fByPos)?true:false;
}

//ѡ�����
UINT CSkinMenu::CheckMenuItem(UINT nMenuID, UINT nCheck)
{
	return m_SkinMenuKernel.CheckMenuItem(nMenuID,nCheck)?true:false;
}

//���ÿ���
UINT CSkinMenu::EnableMenuItem(UINT nMenuID, UINT nEnable)
{
	return m_SkinMenuKernel.EnableMenuItem(nMenuID,nEnable)?true:false;
}

//�ͷ�����
VOID CSkinMenu::FreeMenuItem(CSkinMenuItem * pSkinMenuItem)
{
	//Ч�����
	ASSERT(pSkinMenuItem!=NULL);
	if (pSkinMenuItem==NULL) return;

	//�������
	switch (pSkinMenuItem->m_MenuItemType)
	{
	case MenuItemType_Image:		//ͼ������
		{
			//��������
			CSkinMenuImage * pSkinMenuImage=(CSkinMenuImage *)pSkinMenuItem;

			//���ñ���
			pSkinMenuImage->m_hBitmap=NULL;
			m_MenuItemImage.Add(pSkinMenuImage);

			break;
		}
	case MenuItemType_String:		//�ַ�����
		{
			//��������
			CSkinMenuString * pSkinMenuString=(CSkinMenuString *)pSkinMenuItem;

			//���ñ���
			pSkinMenuString->m_strString.Empty();
			m_MenuItemString.Add(pSkinMenuString);

			break;
		}
	case MenuItemType_Separator:	//�������
		{
			//��������
			CSkinMenuSeparator * pSkinMenuSeparator=(CSkinMenuSeparator *)pSkinMenuItem;

			//���ñ���
			m_MenuItemSeparator.Add(pSkinMenuSeparator);

			break;
		}
	}

	return;
}

//��ȡ����
CSkinMenuItem * CSkinMenu::AcitveMenuItem(enMenuItemType MenuItemType)
{
	//��������
	CSkinMenuItem * pSkinMenuItem=NULL;

	//��������
	switch (MenuItemType)
	{
	case MenuItemType_Image:		//ͼ������
		{
			if (m_MenuItemImage.GetCount()>0)
			{
				INT_PTR nItemCount=m_MenuItemImage.GetCount();
				pSkinMenuItem=m_MenuItemImage[nItemCount-1];
				m_MenuItemImage.RemoveAt(nItemCount-1);
			}
			else
			{
				pSkinMenuItem=new CSkinMenuImage;
				if (pSkinMenuItem==NULL) return NULL;
			}

			break;
		}
	case MenuItemType_String:		//�ַ�����
		{
			if (m_MenuItemString.GetCount()>0)
			{
				INT_PTR nItemCount=m_MenuItemString.GetCount();
				pSkinMenuItem=m_MenuItemString[nItemCount-1];
				m_MenuItemString.RemoveAt(nItemCount-1);
			}
			else
			{
				pSkinMenuItem=new CSkinMenuString;
				if (pSkinMenuItem==NULL) return NULL;
			}

			break;
		}
	case MenuItemType_Separator:	//�������
		{
			if (m_MenuItemSeparator.GetCount()>0)
			{
				INT_PTR nItemCount=m_MenuItemSeparator.GetCount();
				pSkinMenuItem=m_MenuItemSeparator[nItemCount-1];
				m_MenuItemSeparator.RemoveAt(nItemCount-1);
			}
			else
			{
				pSkinMenuItem=new CSkinMenuSeparator;
				if (pSkinMenuItem==NULL) return NULL;
			}

			break;
		}
	default:
		{
			ASSERT(FALSE);
			return NULL;
		}
	}

	//�������
	m_MenuItemActive.Add(pSkinMenuItem);

	return pSkinMenuItem;
}

void CSkinMenu::Release()
{
	int i=0;
	for(i=0;i<(int)m_MenuItemImage.GetCount();i++)
		delete m_MenuItemImage[i];
	for(i=0;i<(int)m_MenuItemString.GetCount();i++)
		delete m_MenuItemString[i];
	for(i=0;i<(int)m_MenuItemSeparator.GetCount();i++)
		delete m_MenuItemSeparator[i];

	m_MenuItemImage.RemoveAll();
	m_MenuItemString.RemoveAll();
	m_MenuItemSeparator.RemoveAll();
}
//////////////////////////////////////////////////////////////////////////////////
