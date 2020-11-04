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

//排序信息
struct tagSortInfo
{
	bool							bAscendSort;						//升序标志
	WORD							wColumnIndex;						//列表索引
	CSkinListCtrl *					pSkinListCtrl;						//列表控件
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

//静态变量
CSkinListCtrlAttribute				CSkinListCtrl::m_SkinAttribute;		//列表属性

//默认属性
#define LIST_STYTE					LVS_EX_DOUBLEBUFFER|LVS_EX_SUBITEMIMAGES|LVS_EX_FULLROWSELECT|LVS_EX_INFOTIP|LVS_EX_ONECLICKACTIVATE

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinListCtrlAttribute::CSkinListCtrlAttribute()
{
	//颜色变量
	m_crTitle = RGB(0,0,0);
	m_crListTX = RGB(10,10,10);
	m_crListBK = RGB(255,255,255);

	return;
}

//析构函数
CSkinListCtrlAttribute::~CSkinListCtrlAttribute()
{
	//删除图片
	m_SkinImageL.Destroy(); 
	m_SkinImageM.Destroy();
	m_SkinImageR.Destroy();
}

//配置资源
bool CSkinListCtrlAttribute::Initialization(CSkinRenderManager * pSkinRenderManager)
{
	//标题属性
	m_crTitle = RGB(0,0,0);
	m_crListTX = RGB(10,10,10);
	m_crListBK = RGB(255,255,255);

	//删除图片
	m_SkinImageL.Destroy(); 
	m_SkinImageM.Destroy();
	m_SkinImageR.Destroy();

	//标题信息
	HINSTANCE hResInstance = GetModuleHandle(SKIN_CONTROLS_DLL_NAME);
	m_SkinImageL.LoadFromResource(hResInstance,IDB_SKIN_HEADER_L);
	m_SkinImageM.LoadFromResource(hResInstance,IDB_SKIN_HEADER_M);
	m_SkinImageR.LoadFromResource(hResInstance,IDB_SKIN_HEADER_R);

	//渲染资源
	pSkinRenderManager->RenderImage(m_SkinImageL);
	pSkinRenderManager->RenderImage(m_SkinImageM);
	pSkinRenderManager->RenderImage(m_SkinImageR);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinHeaderCtrl::CSkinHeaderCtrl()
{
	//设置变量
	m_uLockCount = 0L;
	m_uItemHeight = 130;

	m_nIndex = -1;

	return;
}

//析构函数
CSkinHeaderCtrl::~CSkinHeaderCtrl()
{

}

//控件绑定
VOID CSkinHeaderCtrl::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//创建字体
	CFont Font;
	Font.CreatePointFont(m_uItemHeight,TEXT("宋体"));

	//设置字体
	SetFont(&Font);

	return;
}

//控件消息
BOOL CSkinHeaderCtrl::OnChildNotify(UINT uMessage, WPARAM wParam, LPARAM lParam, LRESULT * pLResult)
{
	//变量定义
	NMHEADER * pNMHearder = (NMHEADER*)lParam;

	//拖动消息
	if ((pNMHearder->hdr.code == HDN_BEGINTRACKA) || (pNMHearder->hdr.code == HDN_BEGINTRACKW)) {
		//禁止拖动
		if (pNMHearder->iItem < (INT)m_uLockCount) {
			*pLResult=TRUE;
			return TRUE;
		}
	}

	return __super::OnChildNotify(uMessage,wParam,lParam,pLResult);
}

//鼠标左键按下
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

//鼠标右键弹起
void CSkinHeaderCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_nIndex = -1;

	__super::OnLButtonUp(nFlags, point);
}

//设置锁定
VOID CSkinHeaderCtrl::SetLockCount(UINT uLockCount)
{
	//设置变量
	m_uLockCount = uLockCount;

	return;
}

//设置列高
VOID CSkinHeaderCtrl::SetItemHeight(UINT uItemHeight)
{
	//设置变量
	m_uItemHeight = uItemHeight;

	//通知修改高度
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

//控件高度
LRESULT CSkinHeaderCtrl::OnLayout(WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = CHeaderCtrl::DefWindowProc(HDM_LAYOUT, wParam, lParam);

	HDLAYOUT& hdLayout = *reinterpret_cast<LPHDLAYOUT>(lParam);
	LPRECT lpRect = hdLayout.prc;
	LPWINDOWPOS lpWindowPos = hdLayout.pwpos;

	lpRect->top = lpWindowPos->cy = 20;

	return lResult;
}


//重画函数
VOID CSkinHeaderCtrl::OnPaint() 
{
	CPaintDC dc(this);

	//获取位置
	CRect rcRect,rcClip;
	GetClientRect(&rcRect);
	dc.GetClipBox(&rcClip);

	//建立缓冲
	CDC BufferDC;
	CBitmap BufferBmp;
	BufferDC.CreateCompatibleDC(&dc);
	BufferBmp.CreateCompatibleBitmap(&dc,rcRect.Width(),rcRect.Height());

	//设置 DC
	BufferDC.SetBkMode(TRANSPARENT);
	BufferDC.SelectObject(&BufferBmp);
	BufferDC.SetTextColor(CSkinListCtrl::m_SkinAttribute.m_crTitle);
	BufferDC.SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//绘画背景
	CSkinListCtrlAttribute * pSkinAttribute=&CSkinListCtrl::m_SkinAttribute;
	pSkinAttribute->m_SkinImageM.StretchBlt(BufferDC,0,0,rcRect.Width(),rcRect.Height());

	//绘画子项
	CRect rcItem;
	HDITEM HDItem;
	TCHAR szBuffer[64];

	for (INT i = 0; i < GetItemCount(); i++) {
		//构造变量
		HDItem.mask = HDI_TEXT;
		HDItem.pszText = szBuffer;
		HDItem.cchTextMax = CountArray(szBuffer);

		//获取信息
		GetItem(i,&HDItem);
		GetItemRect(i,&rcItem);

		//绘画背景
		INT nLeftWidth = pSkinAttribute->m_SkinImageL.GetWidth();
		INT nRightWidth = pSkinAttribute->m_SkinImageR.GetWidth();

		pSkinAttribute->m_SkinImageL.StretchBlt(BufferDC,rcItem.left,rcItem.top,nLeftWidth,rcItem.Height());

		if (m_nIndex == i) {
			pSkinAttribute->m_SkinImageM.StretchBlt(BufferDC,rcItem.left + nLeftWidth,rcItem.bottom,rcItem.Width() - nLeftWidth - nRightWidth,-rcItem.Height()-2);
		} else {
			pSkinAttribute->m_SkinImageM.StretchBlt(BufferDC,rcItem.left + nLeftWidth,rcItem.top,rcItem.Width() - nLeftWidth - nRightWidth,rcItem.Height());
		}
		pSkinAttribute->m_SkinImageR.StretchBlt(BufferDC,rcItem.right - nRightWidth,rcItem.top,nRightWidth,rcItem.Height());

		//绘画标题
		rcItem.DeflateRect(3,1,3,1);
		BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcItem,DT_END_ELLIPSIS|DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	}

	//绘画界面
	dc.BitBlt(0,0,rcRect.Width(),rcRect.Height(),&BufferDC,0,0,SRCCOPY);

	//清理资源
	BufferDC.DeleteDC();
	BufferBmp.DeleteObject();
	
	return;
}

//背景函数
BOOL CSkinHeaderCtrl::OnEraseBkgnd(CDC * pDC) 
{
	//刷新界面
	Invalidate(FALSE);
	UpdateWindow();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinListCtrl::CSkinListCtrl()
{
	//设置变量
	m_bAscendSort = false;
	m_uItemHeight = 0;
	return;
}

//析构函数
CSkinListCtrl::~CSkinListCtrl()
{
}

//控件绑定
VOID CSkinListCtrl::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//设置滚动
	m_SkinScrollBar.InitScroolBar(this);

	//设置颜色
	SetBkColor(m_SkinAttribute.m_crListBK);
	SetTextColor(m_SkinAttribute.m_crListTX);
	SetTextBkColor(m_SkinAttribute.m_crListBK);

	//设置属性
	SetExtendedStyle(LIST_STYTE);
	m_SkinHeaderCtrl.SubclassWindow(GetHeaderCtrl()->GetSafeHwnd());

	return;
}

//设置行高
VOID CSkinListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if( m_uItemHeight > 0 )
	lpMeasureItemStruct->itemHeight = m_uItemHeight;
}

//设置列高
VOID CSkinListCtrl::SetItemHeight(UINT uItemHeight)
{
	//设置变量
	m_uItemHeight = uItemHeight;

	//通知修改高度
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

//绘画函数
VOID CSkinListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//变量定义
	CRect rcItem = lpDrawItemStruct->rcItem;
	CDC * pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	//获取属性
	INT nItemID = lpDrawItemStruct->itemID;
	INT nColumnCount = m_SkinHeaderCtrl.GetItemCount();

	//定制颜色
	COLORREF rcTextColor = RGB(10,10,10);
	COLORREF rcBackColor = RGB(229,249,255);
	GetItemColor(lpDrawItemStruct,rcTextColor,rcBackColor);

	//绘画区域
	CRect rcClipBox;
	pDC->GetClipBox(&rcClipBox);

	//设置环境
	pDC->SetBkColor(rcBackColor);
	pDC->SetTextColor(rcTextColor);

	//绘画背景
	pDC->FillSolidRect(&rcItem,rcBackColor);
	//绘画子项
	for (INT i = 0; i < nColumnCount; i++)
	{
		//获取位置
		CRect rcSubItem;
		GetSubItemRect(nItemID,i,LVIR_BOUNDS,rcSubItem);

		//绘画判断
		if (rcSubItem.left>rcClipBox.right) break;
		if (rcSubItem.right<rcClipBox.left) continue;

		//绘画数据
		DrawCustomItem(pDC,lpDrawItemStruct,rcSubItem,i);
	}

	//绘画焦点
	/*
	if (lpDrawItemStruct->itemState&ODS_FOCUS)
	{
		pDC->DrawFocusRect(&rcItem);
	}
	*/
	return;
}

//建立消息
INT CSkinListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//设置滚动
	m_SkinScrollBar.InitScroolBar(this);

	//设置颜色
	SetBkColor(m_SkinAttribute.m_crListBK);
	SetTextColor(m_SkinAttribute.m_crListTX);
	SetTextBkColor(m_SkinAttribute.m_crListBK);

	//设置属性
	SetExtendedStyle(LIST_STYTE);
	m_SkinHeaderCtrl.SubclassWindow(GetHeaderCtrl()->GetSafeHwnd());

	return 0;
}

//获取位置
INT CSkinListCtrl::GetInsertIndex(VOID * pItemData)
{
	return GetItemCount();
}

//排列数据
INT CSkinListCtrl::SortItemData(LPARAM lParam1, LPARAM lParam2, WORD wColumnIndex, BOOL bAscendSort)
{
	//获取数据
	TCHAR szBuffer1[256]=TEXT("");
	TCHAR szBuffer2[256]=TEXT("");
	GetItemText((INT)lParam1,wColumnIndex,szBuffer1,CountArray(szBuffer1));
	GetItemText((INT)lParam2,wColumnIndex,szBuffer2,CountArray(szBuffer2));

	//对比数据
	INT nResult = lstrcmp(szBuffer1,szBuffer2);
	return (bAscendSort) ? nResult : -nResult;
}

//描述字符
VOID CSkinListCtrl::ConstructString(VOID * pItemData, WORD wColumnIndex, LPTSTR pszString, WORD wMaxCount)
{
	//参数效验
	ASSERT(pszString!=NULL);
	if (pszString==NULL) return;

	//设置变量
	pszString[0]=0;

	return;
}

//获取颜色
VOID CSkinListCtrl::GetItemColor(LPDRAWITEMSTRUCT lpDrawItemStruct, COLORREF & crColorText, COLORREF & crColorBack)
{
	//设置颜色
	if (lpDrawItemStruct->itemState & ODS_SELECTED) {
		//选择颜色
		crColorText = RGB(255,255,255);
		crColorBack = RGB(0xf9,0x37,0x5e);
	} else {
		//设置变量
		crColorText = RGB(10,10,10);
		crColorBack = RGB(255,255,255);
		if ((lpDrawItemStruct->itemID % 2)) {
			crColorBack = RGB(229,249,255);
		}
	}

	return;
}

//绘画数据
VOID CSkinListCtrl::DrawCustomItem(CDC * pDC, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect & rcSubItem, INT nColumnIndex)
{
	//获取文字
	TCHAR szString[256]=TEXT("");
	GetItemText(lpDrawItemStruct->itemID,nColumnIndex,szString,CountArray(szString));

	//绘画文字
	rcSubItem.left+=5;
	pDC->DrawText(szString,lstrlen(szString),&rcSubItem,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	return;
}

//插入数据
bool CSkinListCtrl::InsertDataItem(VOID * pItemData)
{
	//效验状态
	ASSERT((pItemData!=NULL)&&(m_hWnd!=NULL));
	if ((pItemData==NULL)||(m_hWnd==NULL)) return false;

	//更新数据
	if(UpdateDataItem(pItemData)) {
		return true;
	}

	//变量定义
	INT nItemInsert = 0;
	INT nColumnCount = m_SkinHeaderCtrl.GetItemCount();

	//插入数据
	for (INT i = 0; i < nColumnCount; i++) {
		//获取描述
		TCHAR szString[256]=TEXT("");
		ConstructString(pItemData,i,szString,CountArray(szString));

		//插入列表
		if (i == 0) {
			nItemInsert=InsertItem(LVIF_TEXT|LVIF_PARAM,GetInsertIndex(pItemData),szString,0,0,0,(LPARAM)pItemData);
		} else {
			SetItem(nItemInsert,i,LVIF_TEXT,szString,0,0,0,0);
		}
	}

	return true;
}

//更新数据
bool CSkinListCtrl::UpdateDataItem(VOID * pItemData)
{
	//效验状态
	ASSERT((pItemData!=NULL)&&(m_hWnd!=NULL));
	if ((pItemData==NULL)||(m_hWnd==NULL)) return false;
	
	//变量定义
	LVFINDINFO FindInfo;
	ZeroMemory(&FindInfo,sizeof(FindInfo));

	//设置变量
	FindInfo.flags=LVFI_PARAM;
	FindInfo.lParam=(LPARAM)pItemData;

	//查找数据
	INT nItem=FindItem(&FindInfo);
	INT nColumnCount=m_SkinHeaderCtrl.GetItemCount();

	//更新数据
	if (nItem!=-1L)
	{
		//更新数据
		for (INT i=0;i<nColumnCount;i++)
		{
			//获取描述
			TCHAR szString[256]=TEXT("");
			ConstructString(pItemData,i,szString,CountArray(szString));

			//设置列表
			SetItem(nItem,i,LVIF_TEXT,szString,0,0,0,0);
		}

		//更新子项
		RedrawItems(nItem,nItem);
		return true;
	}

	return false;
}

//删除数据
bool CSkinListCtrl::DeleteDataItem(VOID * pItemData)
{
	//效验状态
	ASSERT((pItemData!=NULL)&&(m_hWnd!=NULL));
	if ((pItemData==NULL)||(m_hWnd==NULL)) return false;
	
	//变量定义
	LVFINDINFO FindInfo;
	ZeroMemory(&FindInfo,sizeof(FindInfo));

	//构造变量
	FindInfo.flags=LVFI_PARAM;
	FindInfo.lParam=(LPARAM)pItemData;

	//删除数据
	INT nItem=FindItem(&FindInfo);
	if (nItem!=-1L)
	{
		DeleteItem(nItem);
		return true;
	}

	return false;
}

//绘画函数
void CSkinListCtrl::OnPaint()
{
	CPaintDC dc(this);
	//获取位置
	CRect rcClient,rcClip,rcHeader;
	GetClientRect(&rcClient);

	GetHeaderCtrl()->GetWindowRect(&rcHeader);
	dc.GetClipBox(&rcClip);

	//建立缓冲
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

//绘画背景
BOOL CSkinListCtrl::OnEraseBkgnd(CDC * pDC)
{
	////获取窗口
	//CWnd *pHeader = GetHeaderCtrl();

	////窗口判断
	//if (pHeader->GetSafeHwnd() != NULL)
	//{
	//	//获取位置
	//	CRect rcClient;
	//	GetClientRect(&rcClient);

	//	//获取位置
	//	CRect rcHeader;
	//	pHeader->GetWindowRect(&rcHeader);

	//	//绘画背景
	//	rcClient.top = rcHeader.Height();
	//	pDC->FillSolidRect(&rcClient,GetBkColor());

	//	return FALSE;
	//}

	//return __super::OnEraseBkgnd(pDC);
	return FALSE;
}

//排列函数
INT CALLBACK CSkinListCtrl::SortFunction(LPARAM lParam1, LPARAM lParam2, LPARAM lParamList)
{
	//变量定义
    tagSortInfo * pSortInfo=(tagSortInfo *)lParamList;
	CSkinListCtrl * pSkinListCtrl=pSortInfo->pSkinListCtrl;

	//数据排序
	return pSkinListCtrl->SortItemData(lParam1,lParam2,pSortInfo->wColumnIndex,pSortInfo->bAscendSort);
}

//排序
VOID CSkinListCtrl::SortData(WORD wColumnIndex)
{
	//变量定义
	tagSortInfo SortInfo;
	ZeroMemory(&SortInfo,sizeof(SortInfo));

	//设置变量
	SortInfo.pSkinListCtrl = this;
	SortInfo.bAscendSort = m_bAscendSort;
	SortInfo.wColumnIndex = wColumnIndex;

	//设置变量
	m_bAscendSort = !m_bAscendSort;

	//排列列表
	SortItems(SortFunction,(LPARAM)&SortInfo);
}

//点击消息
VOID CSkinListCtrl::OnColumnclick(NMHDR * pNMHDR, LRESULT * pResult)
{
	//变量定义
	NM_LISTVIEW * pNMListView=(NM_LISTVIEW *)pNMHDR;

	SortData(pNMListView->iSubItem);
	return;
}

//////////////////////////////////////////////////////////////////////////////////
