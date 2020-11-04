#include "StdAfx.h"
#include "Resource.h"
#include "SkinDialog.h"
#include "SkinComboBox.h"
#include "SkinRenderManager.h"

#ifdef _DEBUG
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
	#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////////////////

//时间标识
#define IDI_CHECK_HOVER				10									//过渡标识
#define TIME_CHECK_HOVER			10									//过渡时间

//////////////////////////////////////////////////////////////////////////////////

//静态变量
CSkinComboBoxAttribute			CSkinComboBox::m_SkinAttribute;			//按钮属性

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSkinComboBoxList, CListBox)
	ON_WM_NCPAINT()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CSkinComboBox, CComboBox)
	ON_WM_SIZE()
    ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KILLFOCUS()
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCbnCloseup)
	ON_CONTROL_REFLECT_EX(CBN_SELCHANGE, OnCbnSelChange)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinComboBoxAttribute::CSkinComboBoxAttribute()
{
	//设置变量
	m_uButtonWidth=0;
	m_crEnableBack=CLR_INVALID;
	m_crDisableBack=CLR_INVALID;
	//字体颜色
	m_crEnableText=CLR_INVALID;
	m_crDisableText=CLR_INVALID;

	//边框颜色
	m_crEnableBorad=CLR_INVALID;
	m_crDisableBorad=CLR_INVALID;

	return;
}

//析构函数
CSkinComboBoxAttribute::~CSkinComboBoxAttribute() 
{
}

//配置资源
bool CSkinComboBoxAttribute::Initialization(CSkinRenderManager * pSkinRenderManager)
{
	//设置变量
	m_uButtonWidth=18;
	m_crEnableBack=RGB(255,255,255);
	m_crDisableBack=RGB(125,125,125);
	//字体颜色
	m_crEnableText=RGB(14,14,14);
	m_crDisableText=RGB(180,180,180);

	//边框颜色
	m_crEnableBorad=RGB(102,161,191);
	m_crDisableBorad=RGB(170,170,170);//GetSysColor(COLOR_WINDOW);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinComboBoxList::CSkinComboBoxList()
{
}

//析构函数
CSkinComboBoxList::~CSkinComboBoxList()
{
}

//绘画子项
VOID CSkinComboBoxList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//变量定义
	CRect rcItem=lpDrawItemStruct->rcItem;
	CDC * pDCControl=CDC::FromHandle(lpDrawItemStruct->hDC);

	//创建缓冲
	CDC BufferDC;
	CBitmap ImageBuffer;
	BufferDC.CreateCompatibleDC(pDCControl);
	ImageBuffer.CreateCompatibleBitmap(pDCControl,rcItem.Width(),rcItem.Height());

	//设置环境
	BufferDC.SelectObject(&ImageBuffer);
	BufferDC.SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//获取字符
	CString strString;
	GetText(lpDrawItemStruct->itemID,strString);

	//计算位置
	CRect rcString;
	rcString.SetRect(4,0,rcItem.Width()-8,rcItem.Height());

	//颜色定义
	COLORREF crTextColor=((lpDrawItemStruct->itemState&ODS_SELECTED)!=0)?RGB(255,255,255):RGB(0,0,0);
	COLORREF crBackColor=((lpDrawItemStruct->itemState&ODS_SELECTED)!=0)?RGB(10,36,106):RGB(255,255,255);

	//绘画背景
	BufferDC.FillSolidRect(0,0,rcItem.Width(),rcItem.Height(),crBackColor);

	//绘画字符
	BufferDC.SetBkMode(TRANSPARENT);
	BufferDC.SetTextColor(crTextColor);
	BufferDC.DrawText(strString,&rcString,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	//绘画界面
	pDCControl->BitBlt(rcItem.left,rcItem.top,rcItem.Width(),rcItem.Height(),&BufferDC,0,0,SRCCOPY);

	//清理资源
	BufferDC.DeleteDC();
	ImageBuffer.DeleteObject();

	return;
}

//界面绘画
VOID CSkinComboBoxList::OnNcPaint()
{
	__super::OnNcPaint();

	//获取位置
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//绘画边框
	CWindowDC WindowDC(this);
	COLORREF crBoradFrame=CSkinComboBox::m_SkinAttribute.m_crEnableBorad;
	WindowDC.Draw3dRect(0,0,rcWindow.Width(),rcWindow.Height(),crBoradFrame,crBoradFrame);

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinComboBox::CSkinComboBox()
{
	//控制变量
	m_bHovering=false;
	m_bDrawBorad=true;
	m_bRenderImage=false;
	m_uButtonWidth=m_SkinAttribute.m_uButtonWidth;
	//背景颜色
	m_crEnableBack=m_SkinAttribute.m_crEnableBack;
	m_crDisableBack=m_SkinAttribute.m_crDisableBack;

	//字体颜色
	m_crEnableText=m_SkinAttribute.m_crEnableText;
	m_crDisableText=m_SkinAttribute.m_crDisableText;

	//边框颜色
	m_crEnableBorad=m_SkinAttribute.m_crEnableBorad;
	m_crDisableBorad=m_SkinAttribute.m_crDisableBorad;
	//资源变量
	m_uResourceID=0;
	m_hResInstance=NULL;

	return;
}

//析构函数
CSkinComboBox::~CSkinComboBox()
{
}

//控件绑定
VOID CSkinComboBox::PreSubclassWindow()
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//配置控件
	Initialization();
	RectifyControl(rcClient.Width(),rcClient.Height());

	return;
}

//测量子项
VOID CSkinComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	//设置变量
	lpMeasureItemStruct->itemWidth=0;
	lpMeasureItemStruct->itemHeight=GetItemHeight(LB_ERR);

	return;
}

//绘画子项
VOID CSkinComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	if( lpDrawItemStruct->itemID == -1 )
		return;
}

//绘画控件
VOID CSkinComboBox::DrawControlView(CDC * pDC)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//状态变量
	DWORD dwStyle=GetStyle();
	bool bDisable=(IsWindowEnabled()==FALSE);
	bool bDropped=(GetDroppedState()!=FALSE);

	//绘画背景
	DrawBackGround(pDC,bDisable);

	//绘画边框
	if (m_bDrawBorad==true)
	{
		if (bDisable==false)
		{
			if (m_bRenderImage==true)
			{
				//变量定义
				ASSERT(CSkinRenderManager::GetInstance()!=NULL);
				CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();

				//渲染颜色
				COLORREF crEnableBorad=pSkinRenderManager->RenderColor(0.77,0.65);
				pDC->Draw3dRect(0,0,rcClient.Width(),rcClient.Height(),crEnableBorad,crEnableBorad);
			}
			else
			{
				pDC->Draw3dRect(0,0,rcClient.Width(),rcClient.Height(),m_crEnableBorad,m_crEnableBorad);
			}
		}
		else
		{
			pDC->Draw3dRect(0,0,rcClient.Width(),rcClient.Height(),m_crDisableBorad,m_crDisableBorad);
		}
	}
	else
	{
		pDC->Draw3dRect(0,0,rcClient.Width(),rcClient.Height(),m_crEnableBack,m_crEnableBack);
	}

	//计算位置
	INT nImageIndex=0;
	if (bDisable==true) nImageIndex=4;
	else if (bDropped==true) nImageIndex=1;
	else if (m_bHovering==true) nImageIndex=3;

	//加载按钮
	CBitImage ImageButton;
	if ((m_hResInstance==NULL)||(m_uResourceID==0))
	{
		ImageButton.LoadFromResource(GetModuleHandle(SKIN_CONTROLS_DLL_NAME),IDB_SKIN_COMBOBOX_BUTTON);
	}
	else
	{
		ImageButton.LoadFromResource(m_hResInstance,m_uResourceID);
	}

	//渲染资源
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();
	if ((pSkinRenderManager!=NULL)&&(m_bRenderImage==true)) pSkinRenderManager->RenderImage(ImageButton);

	//绘画按钮
	CSize SizeButton;
	SizeButton.SetSize(ImageButton.GetWidth()/5L,ImageButton.GetHeight());
	LONG lTop = 3, lHeight = rcClient.Height();
	if(lHeight > SizeButton.cy) {
		lTop = (lHeight - SizeButton.cy) / 2;
	}
	ImageButton.StretchBlt(pDC->m_hDC, rcClient.Width() - m_uButtonWidth - 1,lTop,
		SizeButton.cx,SizeButton.cy,nImageIndex * SizeButton.cx,0,SizeButton.cx,SizeButton.cy);

	return;
}

void CSkinComboBox::DrawBackGround( CDC *pDC,bool bDisable )
{
	CRect rcClient;
	GetClientRect(&rcClient);

	DWORD dwStyle=GetStyle();

	if (((dwStyle & 0x000F)==CBS_DROPDOWN)||(GetCurSel()==LB_ERR))
	{
		if (bDisable==false)
		{
			pDC->FillSolidRect(1,1,rcClient.Width(),rcClient.Height(),m_crEnableBack);
		}
		else
		{
			pDC->FillSolidRect(1,1,rcClient.Width(),rcClient.Height(),m_crEnableBack);
		}
	}

	//绘画背景
	if ((dwStyle & 0x000F)==CBS_DROPDOWNLIST)
	{
		//设置颜色
		if (GetFocus()->GetSafeHwnd()==m_hWnd)
		{
			pDC->FillSolidRect(0,0,rcClient.Width(),rcClient.Height(),m_crEnableBack);
		}
		else
		{
			pDC->FillSolidRect(0,0,rcClient.Width(),rcClient.Height(),m_crEnableBack);
		}
	
		//绘画字符
		CString strString;
		GetWindowText(strString);

		//获取字符
		CRect rcText;
		GetClientRect(&rcText);

		//设置环境
		pDC->SetBkMode(TRANSPARENT);
		pDC->SelectObject(GetFont());

		if (bDisable==false)
			pDC->SetTextColor(m_crEnableText);
		else
			pDC->SetTextColor(m_crDisableText);

		//绘画字符
		rcText.left += 4;
		pDC->DrawText(strString,&rcText,DT_VCENTER|DT_SINGLELINE);
	}
}

//失去焦点
VOID CSkinComboBox::OnKillFocus(CWnd *pWnd)
{
	//更新界面
	if (m_hWnd!=NULL) Invalidate(FALSE);
}

//按钮检测
bool CSkinComboBox::VerdictOverButton(CPoint MousePoint)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//下拉列表
	if ((rcClient.PtInRect(MousePoint)==TRUE)&&((GetStyle()&CBS_SIMPLE)!=0)) return true;

	//坐标计算
	if ((MousePoint.y>(rcClient.Height()-1))||(MousePoint.y<1)) return false;
	if ((MousePoint.x<(rcClient.Width()-(INT)m_uButtonWidth-1))||(MousePoint.x>(rcClient.Width()-1))) return false;

	return true;
}

//边框设置
VOID CSkinComboBox::SetDrawBorad(bool bDrawBorad)
{
	//设置变量
	m_bDrawBorad=bDrawBorad;

	//更新界面
	if (m_hWnd!=NULL) Invalidate(FALSE);

	return;
}

//设置渲染
VOID CSkinComboBox::SetRenderImage(bool bRenderImage)
{
	//设置变量
	m_bRenderImage=bRenderImage;

	//更新界面
	if (m_hWnd!=NULL) Invalidate(FALSE);

	return;
}

//设置资源
VOID CSkinComboBox::SetButtonImage(HINSTANCE hResInstance, UINT uResourceID, CSize & SizeButton)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//设置变量
	m_uResourceID=uResourceID;
	m_hResInstance=hResInstance;

	//设置控件
	m_uButtonWidth=SizeButton.cx;
	SetItemHeight(LB_ERR,SizeButton.cy);

	//配置控件
	RectifyControl(rcClient.Width(),rcClient.Height());

	//更新界面
	if (m_hWnd!=NULL) RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW);

	return;
}

//配置控件
VOID CSkinComboBox::Initialization()
{
	//设置状态
	m_bHovering=false;
	m_uButtonWidth=m_SkinAttribute.m_uButtonWidth;

	//变量定义
	COMBOBOXINFO ComboBoxInfo;
	ComboBoxInfo.cbSize=sizeof(ComboBoxInfo);

	//绑定控件
	if (GetComboBoxInfo(&ComboBoxInfo)==TRUE)
	{
		if (ComboBoxInfo.hwndItem!=NULL) m_SkinComboBoxEdit.SubclassWindow(ComboBoxInfo.hwndItem);
		if (ComboBoxInfo.hwndList!=NULL) m_SkinComboBoxList.SubclassWindow(ComboBoxInfo.hwndList);
	}

	return;
}

//更新按钮
VOID CSkinComboBox::RedrawDropButton()
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//按钮位置
	CRect rcDropButton;
	rcDropButton.SetRect(rcClient.Width()-m_uButtonWidth-1,1,rcClient.Width()-1,rcClient.Height()-1);

	//更新界面
	RedrawWindow(&rcDropButton,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW);

	return;
}

//调整控件
VOID CSkinComboBox::RectifyControl(INT nWidth, INT nHeight)
{
	//移动控件
	if (m_SkinComboBoxEdit.m_hWnd!=NULL)
	{
		//获取位置
		CRect rcClient;
		GetClientRect(&rcClient);

		//移动控件
		CSize SizeEdit;
		SizeEdit.SetSize(rcClient.Width()-m_uButtonWidth-10,12);
		m_SkinComboBoxEdit.SetWindowPos(NULL,2,(rcClient.Height()-12)/2,SizeEdit.cx,SizeEdit.cy,SWP_NOZORDER);
	}

	return;
}

//重画消息
VOID CSkinComboBox::OnPaint()
{
	CPaintDC dc(this);
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//建立缓冲
	CImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//创建 DC
	CImageDC ImageDC(ImageBuffer);
	CDC * pBufferDC = CDC::FromHandle(ImageDC);

	//绘画界面
	DrawControlView(pBufferDC);

	//绘画界面
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return;
}

//列表关闭
VOID CSkinComboBox::OnCbnCloseup()
{
	//更新按钮
	RedrawDropButton();

	//更新界面
	if (m_hWnd!=NULL) Invalidate(FALSE);

	return;
}

//更改事件
BOOL CSkinComboBox::OnCbnSelChange()
{
	//更新界面
	if (m_hWnd!=NULL) Invalidate(FALSE);

	return 0;
}

//绘画背景
BOOL CSkinComboBox::OnEraseBkgnd(CDC * pDC)
{
	return FALSE;
}

//时间消息
VOID CSkinComboBox::OnTimer(UINT_PTR nIDEvent)
{
	//盘旋判断
	if (nIDEvent==IDI_CHECK_HOVER)
	{
		//盘旋判断
		if (m_bHovering==true)
		{
			//获取光标
			CPoint MousePoint;
			GetCursorPos(&MousePoint);
			ScreenToClient(&MousePoint);

			//盘旋判断
			m_bHovering=VerdictOverButton(MousePoint);

			//更新界面
			if (m_bHovering==false) RedrawDropButton();
		}

		//删除时间
		if (m_bHovering==false) KillTimer(IDI_CHECK_HOVER);

		return;
	}

	__super::OnTimer(nIDEvent);
}

//位置消息
VOID CSkinComboBox::OnSize(UINT nType, INT cx, INT cy) 
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	RectifyControl(cx,cy);

	return;
}

//创建消息
INT CSkinComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//配置控件
	Initialization();
	RectifyControl(rcClient.Width(),rcClient.Height());

	return 0;
}

//设置颜色
VOID CSkinComboBox::SetEnableColor(COLORREF crEnableText, COLORREF crEnableBK, COLORREF crEnableBorad)
{
	//设置颜色
	m_crEnableBack=crEnableBK;
	m_crEnableText=crEnableText;
	m_crEnableBorad=crEnableBorad;

	m_SkinComboBoxEdit.SetEnableColor(crEnableText,crEnableBK,crEnableBorad);
	//更新界面
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//设置颜色
VOID CSkinComboBox::SetDisableColor(COLORREF crDisableText, COLORREF crDisableBK, COLORREF crDisableBorad)
{
	//设置颜色
	m_crDisableBack=crDisableBK;
	m_crDisableText=crDisableText;
	m_crDisableBorad=crDisableBorad;

	m_SkinComboBoxEdit.SetDisableColor(crDisableText,crDisableBK,crDisableBorad);
	//更新界面
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//鼠标消息
VOID CSkinComboBox::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);

	if (m_bHovering==false)
	{
		//盘旋判断
		m_bHovering=VerdictOverButton(Point);

		//状态判断
		if (m_bHovering==true)
		{
			//更新界面
			RedrawDropButton();

			//设置时间
			SetTimer(IDI_CHECK_HOVER,TIME_CHECK_HOVER,NULL);
		}
	}

	return;
}

//鼠标消息
VOID CSkinComboBox::OnLButtonDown(UINT nFlags, CPoint Point)
{
	//设置焦点
	SetFocus();

	//显示列表
	if (VerdictOverButton(Point)==true)
	{
		//显示列表
		ShowDropDown(GetDroppedState()==FALSE);

		//更新按钮
		RedrawDropButton();
	}

	return;
}

//鼠标消息
VOID CSkinComboBox::OnLButtonDblClk(UINT nFlags, CPoint Point)
{
	//设置焦点
	SetFocus();

	//显示列表
	if (VerdictOverButton(Point)==true)
	{
		//显示列表
		ShowDropDown(GetDroppedState()==FALSE);

		//更新按钮
		RedrawDropButton();
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////