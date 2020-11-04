#include "Stdafx.h"
#include "Resource.h"
#include "SkinListCtrl.h"
#include "SkinResourceManager.h"

#ifdef _DEBUG
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
	#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////////////////

//������Ϣ
struct tagSortInfo
{
	bool							bAscendSort;						//�����־
	WORD							wColumnIndex;						//�б�����
	CSkinListCtrl *					pSkinListCtrl;						//�б�ؼ�
};

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSkinHeaderCtrl, CHeaderCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(HDM_LAYOUT, OnLayout)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CSkinListCtrl, CListCtrl)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_MEASUREITEM_REFLECT()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//��̬����
CSkinListCtrlAttribute				CSkinListCtrl::m_SkinAttribute;		//�б�����

//Ĭ������
#define LIST_STYTE					LVS_EX_DOUBLEBUFFER|LVS_EX_SUBITEMIMAGES|LVS_EX_FULLROWSELECT|LVS_EX_INFOTIP|LVS_EX_ONECLICKACTIVATE

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CSkinListCtrlAttribute::CSkinListCtrlAttribute()
{
	//��ɫ����
	m_crTitle = RGB(0,0,0);
	m_crListTX = RGB(10,10,10);
	m_crListBK = RGB(255,255,255);

	return;
}

//��������
CSkinListCtrlAttribute::~CSkinListCtrlAttribute()
{
	//ɾ��ͼƬ
	m_SkinImageL.Destroy(); 
	m_SkinImageM.Destroy();
	m_SkinImageR.Destroy();
}

//������Դ
bool CSkinListCtrlAttribute::Initialization(CSkinRenderManager * pSkinRenderManager)
{
	//��������
	m_crTitle = RGB(0,0,0);
	m_crListTX = RGB(10,10,10);
	m_crListBK = RGB(255,255,255);

	//ɾ��ͼƬ
	m_SkinImageL.Destroy(); 
	m_SkinImageM.Destroy();
	m_SkinImageR.Destroy();

	//������Ϣ
	HINSTANCE hResInstance = GetModuleHandle(SKIN_CONTROLS_DLL_NAME);
	m_SkinImageL.LoadFromResource(hResInstance,IDB_SKIN_HEADER_L);
	m_SkinImageM.LoadFromResource(hResInstance,IDB_SKIN_HEADER_M);
	m_SkinImageR.LoadFromResource(hResInstance,IDB_SKIN_HEADER_R);

	//��Ⱦ��Դ
	pSkinRenderManager->RenderImage(m_SkinImageL);
	pSkinRenderManager->RenderImage(m_SkinImageM);
	pSkinRenderManager->RenderImage(m_SkinImageR);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CSkinHeaderCtrl::CSkinHeaderCtrl()
{
	//���ñ���
	m_uLockCount = 0L;
	m_uItemHeight = 130;

	m_nIndex = -1;

	return;
}

//��������
CSkinHeaderCtrl::~CSkinHeaderCtrl()
{

}

//�ؼ���
VOID CSkinHeaderCtrl::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//��������
	CFont Font;
	Font.CreatePointFont(m_uItemHeight,TEXT("����"));

	//��������
	SetFont(&Font);

	return;
}

//�ؼ���Ϣ
BOOL CSkinHeaderCtrl::OnChildNotify(UINT uMessage, WPARAM wParam, LPARAM lParam, LRESULT * pLResult)
{
	//��������
	NMHEADER * pNMHearder = (NMHEADER*)lParam;

	//�϶���Ϣ
	if ((pNMHearder->hdr.code == HDN_BEGINTRACKA) || (pNMHearder->hdr.code == HDN_BEGINTRACKW)) {
		//��ֹ�϶�
		if (pNMHearder->iItem < (INT)m_uLockCount) {
			*pLResult=TRUE;
			return TRUE;
		}
	}

	return __super::OnChildNotify(uMessage,wParam,lParam,pLResult);
}

//����������
void CSkinHeaderCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rcItem;
	m_nIndex = -1;

	for (INT i = 0; i < GetItemCount(); i++) {
		GetItemRect(i,&rcItem);
		if (rcItem.PtInRect(point)) {
			m_nIndex = i;
			break;
		}
	}
	__super::OnLButtonDown(nFlags, point);
}

//����Ҽ�����
void CSkinHeaderCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_nIndex = -1;

	__super::OnLButtonUp(nFlags, point);
}

//��������
VOID CSkinHeaderCtrl::SetLockCount(UINT uLockCount)
{
	//���ñ���
	m_uLockCount = uLockCount;

	return;
}

//�����и�
VOID CSkinHeaderCtrl::SetItemHeight(UINT uItemHeight)
{
	//���ñ���
	m_uItemHeight = uItemHeight;

	//֪ͨ�޸ĸ߶�
	CRect rcWin;
	GetWindowRect(&rcWin);
	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rcWin.Width();
	wp.cy = rcWin.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);
	return;
}

//�ؼ��߶�
LRESULT CSkinHeaderCtrl::OnLayout(WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = CHeaderCtrl::DefWindowProc(HDM_LAYOUT, wParam, lParam);

	HDLAYOUT& hdLayout = *reinterpret_cast<LPHDLAYOUT>(lParam);
	LPRECT lpRect = hdLayout.prc;
	LPWINDOWPOS lpWindowPos = hdLayout.pwpos;

	lpRect->top = lpWindowPos->cy = 20;

	return lResult;
}


//�ػ�����
VOID CSkinHeaderCtrl::OnPaint() 
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcRect,rcClip;
	GetClientRect(&rcRect);
	dc.GetClipBox(&rcClip);

	//��������
	CDC BufferDC;
	CBitmap BufferBmp;
	BufferDC.CreateCompatibleDC(&dc);
	BufferBmp.CreateCompatibleBitmap(&dc,rcRect.Width(),rcRect.Height());

	//���� DC
	BufferDC.SetBkMode(TRANSPARENT);
	BufferDC.SelectObject(&BufferBmp);
	BufferDC.SetTextColor(CSkinListCtrl::m_SkinAttribute.m_crTitle);
	BufferDC.SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//�滭����
	CSkinListCtrlAttribute * pSkinAttribute=&CSkinListCtrl::m_SkinAttribute;
	pSkinAttribute->m_SkinImageM.StretchBlt(BufferDC,0,0,rcRect.Width(),rcRect.Height());

	//�滭����
	CRect rcItem;
	HDITEM HDItem;
	TCHAR szBuffer[64];

	for (INT i = 0; i < GetItemCount(); i++) {
		//�������
		HDItem.mask = HDI_TEXT;
		HDItem.pszText = szBuffer;
		HDItem.cchTextMax = CountArray(szBuffer);

		//��ȡ��Ϣ
		GetItem(i,&HDItem);
		GetItemRect(i,&rcItem);

		//�滭����
		INT nLeftWidth = pSkinAttribute->m_SkinImageL.GetWidth();
		INT nRightWidth = pSkinAttribute->m_SkinImageR.GetWidth();

		pSkinAttribute->m_SkinImageL.StretchBlt(BufferDC,rcItem.left,rcItem.top,nLeftWidth,rcItem.Height());

		if (m_nIndex == i) {
			pSkinAttribute->m_SkinImageM.StretchBlt(BufferDC,rcItem.left + nLeftWidth,rcItem.bottom,rcItem.Width() - nLeftWidth - nRightWidth,-rcItem.Height()-2);
		} else {
			pSkinAttribute->m_SkinImageM.StretchBlt(BufferDC,rcItem.left + nLeftWidth,rcItem.top,rcItem.Width() - nLeftWidth - nRightWidth,rcItem.Height());
		}
		pSkinAttribute->m_SkinImageR.StretchBlt(BufferDC,rcItem.right - nRightWidth,rcItem.top,nRightWidth,rcItem.Height());

		//�滭����
		rcItem.DeflateRect(3,1,3,1);
		BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcItem,DT_END_ELLIPSIS|DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	}

	//�滭����
	dc.BitBlt(0,0,rcRect.Width(),rcRect.Height(),&BufferDC,0,0,SRCCOPY);

	//������Դ
	BufferDC.DeleteDC();
	BufferBmp.DeleteObject();
	
	return;
}

//��������
BOOL CSkinHeaderCtrl::OnEraseBkgnd(CDC * pDC) 
{
	//ˢ�½���
	Invalidate(FALSE);
	UpdateWindow();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CSkinListCtrl::CSkinListCtrl()
{
	//���ñ���
	m_bAscendSort = false;
	m_uItemHeight = 0;
	return;
}

//��������
CSkinListCtrl::~CSkinListCtrl()
{
}

//�ؼ���
VOID CSkinListCtrl::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//���ù���
	m_SkinScrollBar.InitScroolBar(this);

	//������ɫ
	SetBkColor(m_SkinAttribute.m_crListBK);
	SetTextColor(m_SkinAttribute.m_crListTX);
	SetTextBkColor(m_SkinAttribute.m_crListBK);

	//��������
	SetExtendedStyle(LIST_STYTE);
	m_SkinHeaderCtrl.SubclassWindow(GetHeaderCtrl()->GetSafeHwnd());

	return;
}

//�����и�
VOID CSkinListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if( m_uItemHeight > 0 )
	lpMeasureItemStruct->itemHeight = m_uItemHeight;
}

//�����и�
VOID CSkinListCtrl::SetItemHeight(UINT uItemHeight)
{
	//���ñ���
	m_uItemHeight = uItemHeight;

	//֪ͨ�޸ĸ߶�
	CRect rcWin;
	GetWindowRect(&rcWin);
	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rcWin.Width();
	wp.cy = rcWin.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);
	return;
}

//�滭����
VOID CSkinListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//��������
	CRect rcItem = lpDrawItemStruct->rcItem;
	CDC * pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	//��ȡ����
	INT nItemID = lpDrawItemStruct->itemID;
	INT nColumnCount = m_SkinHeaderCtrl.GetItemCount();

	//������ɫ
	COLORREF rcTextColor = RGB(10,10,10);
	COLORREF rcBackColor = RGB(229,249,255);
	GetItemColor(lpDrawItemStruct,rcTextColor,rcBackColor);

	//�滭����
	CRect rcClipBox;
	pDC->GetClipBox(&rcClipBox);

	//���û���
	pDC->SetBkColor(rcBackColor);
	pDC->SetTextColor(rcTextColor);

	//�滭����
	pDC->FillSolidRect(&rcItem,rcBackColor);
	//�滭����
	for (INT i = 0; i < nColumnCount; i++)
	{
		//��ȡλ��
		CRect rcSubItem;
		GetSubItemRect(nItemID,i,LVIR_BOUNDS,rcSubItem);

		//�滭�ж�
		if (rcSubItem.left>rcClipBox.right) break;
		if (rcSubItem.right<rcClipBox.left) continue;

		//�滭����
		DrawCustomItem(pDC,lpDrawItemStruct,rcSubItem,i);
	}

	//�滭����
	/*
	if (lpDrawItemStruct->itemState&ODS_FOCUS)
	{
		pDC->DrawFocusRect(&rcItem);
	}
	*/
	return;
}

//������Ϣ
INT CSkinListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//���ù���
	m_SkinScrollBar.InitScroolBar(this);

	//������ɫ
	SetBkColor(m_SkinAttribute.m_crListBK);
	SetTextColor(m_SkinAttribute.m_crListTX);
	SetTextBkColor(m_SkinAttribute.m_crListBK);

	//��������
	SetExtendedStyle(LIST_STYTE);
	m_SkinHeaderCtrl.SubclassWindow(GetHeaderCtrl()->GetSafeHwnd());

	return 0;
}

//��ȡλ��
INT CSkinListCtrl::GetInsertIndex(VOID * pItemData)
{
	return GetItemCount();
}

//��������
INT CSkinListCtrl::SortItemData(LPARAM lParam1, LPARAM lParam2, WORD wColumnIndex, BOOL bAscendSort)
{
	//��ȡ����
	TCHAR szBuffer1[256]=TEXT("");
	TCHAR szBuffer2[256]=TEXT("");
	GetItemText((INT)lParam1,wColumnIndex,szBuffer1,CountArray(szBuffer1));
	GetItemText((INT)lParam2,wColumnIndex,szBuffer2,CountArray(szBuffer2));

	//�Ա�����
	INT nResult = lstrcmp(szBuffer1,szBuffer2);
	return (bAscendSort) ? nResult : -nResult;
}

//�����ַ�
VOID CSkinListCtrl::ConstructString(VOID * pItemData, WORD wColumnIndex, LPTSTR pszString, WORD wMaxCount)
{
	//����Ч��
	ASSERT(pszString!=NULL);
	if (pszString==NULL) return;

	//���ñ���
	pszString[0]=0;

	return;
}

//��ȡ��ɫ
VOID CSkinListCtrl::GetItemColor(LPDRAWITEMSTRUCT lpDrawItemStruct, COLORREF & crColorText, COLORREF & crColorBack)
{
	//������ɫ
	if (lpDrawItemStruct->itemState & ODS_SELECTED) {
		//ѡ����ɫ
		crColorText = RGB(255,255,255);
		crColorBack = RGB(0xf9,0x37,0x5e);
	} else {
		//���ñ���
		crColorText = RGB(10,10,10);
		crColorBack = RGB(255,255,255);
		if ((lpDrawItemStruct->itemID % 2)) {
			crColorBack = RGB(229,249,255);
		}
	}

	return;
}

//�滭����
VOID CSkinListCtrl::DrawCustomItem(CDC * pDC, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect & rcSubItem, INT nColumnIndex)
{
	//��ȡ����
	TCHAR szString[256]=TEXT("");
	GetItemText(lpDrawItemStruct->itemID,nColumnIndex,szString,CountArray(szString));

	//�滭����
	rcSubItem.left+=5;
	pDC->DrawText(szString,lstrlen(szString),&rcSubItem,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	return;
}

//��������
bool CSkinListCtrl::InsertDataItem(VOID * pItemData)
{
	//Ч��״̬
	ASSERT((pItemData!=NULL)&&(m_hWnd!=NULL));
	if ((pItemData==NULL)||(m_hWnd==NULL)) return false;

	//��������
	if(UpdateDataItem(pItemData)) {
		return true;
	}

	//��������
	INT nItemInsert = 0;
	INT nColumnCount = m_SkinHeaderCtrl.GetItemCount();

	//��������
	for (INT i = 0; i < nColumnCount; i++) {
		//��ȡ����
		TCHAR szString[256]=TEXT("");
		ConstructString(pItemData,i,szString,CountArray(szString));

		//�����б�
		if (i == 0) {
			nItemInsert=InsertItem(LVIF_TEXT|LVIF_PARAM,GetInsertIndex(pItemData),szString,0,0,0,(LPARAM)pItemData);
		} else {
			SetItem(nItemInsert,i,LVIF_TEXT,szString,0,0,0,0);
		}
	}

	return true;
}

//��������
bool CSkinListCtrl::UpdateDataItem(VOID * pItemData)
{
	//Ч��״̬
	ASSERT((pItemData!=NULL)&&(m_hWnd!=NULL));
	if ((pItemData==NULL)||(m_hWnd==NULL)) return false;
	
	//��������
	LVFINDINFO FindInfo;
	ZeroMemory(&FindInfo,sizeof(FindInfo));

	//���ñ���
	FindInfo.flags=LVFI_PARAM;
	FindInfo.lParam=(LPARAM)pItemData;

	//��������
	INT nItem=FindItem(&FindInfo);
	INT nColumnCount=m_SkinHeaderCtrl.GetItemCount();

	//��������
	if (nItem!=-1L)
	{
		//��������
		for (INT i=0;i<nColumnCount;i++)
		{
			//��ȡ����
			TCHAR szString[256]=TEXT("");
			ConstructString(pItemData,i,szString,CountArray(szString));

			//�����б�
			SetItem(nItem,i,LVIF_TEXT,szString,0,0,0,0);
		}

		//��������
		RedrawItems(nItem,nItem);
		return true;
	}

	return false;
}

//ɾ������
bool CSkinListCtrl::DeleteDataItem(VOID * pItemData)
{
	//Ч��״̬
	ASSERT((pItemData!=NULL)&&(m_hWnd!=NULL));
	if ((pItemData==NULL)||(m_hWnd==NULL)) return false;
	
	//��������
	LVFINDINFO FindInfo;
	ZeroMemory(&FindInfo,sizeof(FindInfo));

	//�������
	FindInfo.flags=LVFI_PARAM;
	FindInfo.lParam=(LPARAM)pItemData;

	//ɾ������
	INT nItem=FindItem(&FindInfo);
	if (nItem!=-1L)
	{
		DeleteItem(nItem);
		return true;
	}

	return false;
}

//�滭����
void CSkinListCtrl::OnPaint()
{
	CPaintDC dc(this);
	//��ȡλ��
	CRect rcClient,rcClip,rcHeader;
	GetClientRect(&rcClient);

	GetHeaderCtrl()->GetWindowRect(&rcHeader);
	dc.GetClipBox(&rcClip);

	//��������
	CDC BufferDC;
	CBitmap BufferBmp, *pOldBitmap;

	BufferDC.CreateCompatibleDC(&dc);
	int nMode = BufferDC.SetMapMode(dc.GetMapMode());

	BufferBmp.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());
	pOldBitmap = BufferDC.SelectObject(&BufferBmp);
	BufferDC.FillSolidRect(&rcClient,GetBkColor());

	CRgn rgn;
	rgn.CreateRectRgn(rcClip.left,rcClip.top,rcClip.right,rcClip.bottom);
	BufferDC.SelectClipRgn(&rgn);
	DefWindowProc(WM_PAINT,(WPARAM)BufferDC.m_hDC,(LPARAM)0);
	BufferDC.SelectClipRgn(NULL);

	if (rcClip.top < rcHeader.Height())
		rcClip.top = rcHeader.Height();

	dc.BitBlt(rcClip.left,rcClip.top,rcClip.Width(),rcClip.Height(),&BufferDC,rcClip.left,rcClip.top,SRCCOPY);

	BufferDC.SetMapMode(nMode);
	BufferDC.SelectObject(pOldBitmap);
	BufferDC.DeleteDC();
	BufferBmp.DeleteObject();
	rgn.DeleteObject();
}

//�滭����
BOOL CSkinListCtrl::OnEraseBkgnd(CDC * pDC)
{
	////��ȡ����
	//CWnd *pHeader = GetHeaderCtrl();

	////�����ж�
	//if (pHeader->GetSafeHwnd() != NULL)
	//{
	//	//��ȡλ��
	//	CRect rcClient;
	//	GetClientRect(&rcClient);

	//	//��ȡλ��
	//	CRect rcHeader;
	//	pHeader->GetWindowRect(&rcHeader);

	//	//�滭����
	//	rcClient.top = rcHeader.Height();
	//	pDC->FillSolidRect(&rcClient,GetBkColor());

	//	return FALSE;
	//}

	//return __super::OnEraseBkgnd(pDC);
	return FALSE;
}

//���к���
INT CALLBACK CSkinListCtrl::SortFunction(LPARAM lParam1, LPARAM lParam2, LPARAM lParamList)
{
	//��������
    tagSortInfo * pSortInfo=(tagSortInfo *)lParamList;
	CSkinListCtrl * pSkinListCtrl=pSortInfo->pSkinListCtrl;

	//��������
	return pSkinListCtrl->SortItemData(lParam1,lParam2,pSortInfo->wColumnIndex,pSortInfo->bAscendSort);
}

//����
VOID CSkinListCtrl::SortData(WORD wColumnIndex)
{
	//��������
	tagSortInfo SortInfo;
	ZeroMemory(&SortInfo,sizeof(SortInfo));

	//���ñ���
	SortInfo.pSkinListCtrl = this;
	SortInfo.bAscendSort = m_bAscendSort;
	SortInfo.wColumnIndex = wColumnIndex;

	//���ñ���
	m_bAscendSort = !m_bAscendSort;

	//�����б�
	SortItems(SortFunction,(LPARAM)&SortInfo);
}

//�����Ϣ
VOID CSkinListCtrl::OnColumnclick(NMHDR * pNMHDR, LRESULT * pResult)
{
	//��������
	NM_LISTVIEW * pNMListView=(NM_LISTVIEW *)pNMHDR;

	SortData(pNMListView->iSubItem);
	return;
}

//////////////////////////////////////////////////////////////////////////////////
