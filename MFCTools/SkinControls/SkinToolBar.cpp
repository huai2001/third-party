#include "stdafx.h"
#include "SkinToolBar.h"

#if defined(_DEBUG) && defined(_MSC_VER)
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
//构造函数
CSkinToolBar::CSkinToolBar()
{
	m_bDropDown = FALSE;
	return;
}

//析构函数
CSkinToolBar::~CSkinToolBar()
{
	return;
}


BEGIN_MESSAGE_MAP(CSkinToolBar, CToolBar)
	//{{AFX_MSG_MAP(CSkinToolBar)
	ON_NOTIFY_REFLECT(TBN_DROPDOWN, OnToolbarDropDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
BOOL CSkinToolBar::SetToolBarButton(INT nBtnWidth,
											 UINT uToolBar,
											 UINT uToolBarHot,
											 UINT uToolBarDisabled)
{
	if (!SetToolBar(TB_SETIMAGELIST, uToolBar, nBtnWidth))
		return FALSE;
	
	if (uToolBarHot) {
		if (!SetToolBar(TB_SETHOTIMAGELIST, uToolBarHot, nBtnWidth))
			return FALSE;
	}

	if (uToolBarDisabled) {
		if (!SetToolBar(TB_SETDISABLEDIMAGELIST, uToolBarDisabled, nBtnWidth))
			return FALSE;
	}

	return TRUE;
}


BOOL CSkinToolBar::SetToolBar(UINT uToolBarType, 
							     	        UINT uToolBar,
										    int  nBtnWidth)
{
	CImageList ImageList;
	CBitmap Bitmap;
	BITMAP bmBitmap;
	
	if (!Bitmap.Attach(LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(uToolBar),
								  IMAGE_BITMAP, 0, 0,
								  LR_DEFAULTSIZE|LR_CREATEDIBSECTION)) ||
	    !Bitmap.GetBitmap(&bmBitmap))
		return FALSE;

	CSize cSize(bmBitmap.bmWidth, bmBitmap.bmHeight); 
	int nNbBtn	= cSize.cx / nBtnWidth;
	RGBTRIPLE* rgb = (RGBTRIPLE*)(bmBitmap.bmBits);
	COLORREF rgbMask = RGB(rgb[0].rgbtRed, rgb[0].rgbtGreen, rgb[0].rgbtBlue);
	
	if (!ImageList.Create(nBtnWidth, cSize.cy, ILC_COLOR24|ILC_MASK, nNbBtn, 0))
		return FALSE;
	
	if (ImageList.Add(&Bitmap, rgbMask) == -1)
		return FALSE;

	SendMessage(uToolBarType, 0, (LPARAM)ImageList.m_hImageList);
	ImageList.Detach(); 
	Bitmap.Detach();
	
	return TRUE;
}

void CSkinToolBar::AddDropDownButton(CWnd* pParent, UINT uButtonID, UINT uMenuID)
{
	if (!m_bDropDown) {
		GetToolBarCtrl().SendMessage(TB_SETEXTENDEDSTYLE, 0, (LPARAM)TBSTYLE_EX_DRAWDDARROWS);
		m_bDropDown = TRUE;
	}

	SetButtonStyle(CommandToIndex(uButtonID), TBSTYLE_DROPDOWN);

	tagDropDownInfo DropDownInfo;
	DropDownInfo.pParent	= pParent;
	DropDownInfo.uButtonID	= uButtonID;
	DropDownInfo.uMenuID	= uMenuID;
	m_DropDownButton.Add(DropDownInfo);
}

void CSkinToolBar::OnToolbarDropDown(NMHDR* pnmtb, LRESULT *plr)
{
	INT_PTR nCount = m_DropDownButton.GetSize();
	for (INT_PTR i = 0; i < nCount; i++) {
		
		tagDropDownInfo DropDownInfo = m_DropDownButton.GetAt(i);

		if (DropDownInfo.uButtonID == UINT(pnmtb->idFrom)) {

			CMenu menu;
			menu.LoadMenu(DropDownInfo.uMenuID);
			CMenu* pPopup = menu.GetSubMenu(0);
			
			CRect rc;
			SendMessage(TB_GETRECT, (WPARAM)pnmtb->idFrom, (LPARAM)&rc);
			ClientToScreen(&rc);
			
			pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_VERTICAL,rc.left, rc.bottom, DropDownInfo.pParent, &rc);
			break;
		}
	}
}