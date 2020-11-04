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

//ʱ���ʶ
#define IDI_CHECK_HOVER				10									//���ɱ�ʶ
#define TIME_CHECK_HOVER			10									//����ʱ��

//////////////////////////////////////////////////////////////////////////////////

//��̬����
CSkinComboBoxAttribute			CSkinComboBox::m_SkinAttribute;			//��ť����

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

//���캯��
CSkinComboBoxAttribute::CSkinComboBoxAttribute()
{
	//���ñ���
	m_uButtonWidth=0;
	m_crEnableBack=CLR_INVALID;
	m_crDisableBack=CLR_INVALID;
	//������ɫ
	m_crEnableText=CLR_INVALID;
	m_crDisableText=CLR_INVALID;

	//�߿���ɫ
	m_crEnableBorad=CLR_INVALID;
	m_crDisableBorad=CLR_INVALID;

	return;
}

//��������
CSkinComboBoxAttribute::~CSkinComboBoxAttribute() 
{
}

//������Դ
bool CSkinComboBoxAttribute::Initialization(CSkinRenderManager * pSkinRenderManager)
{
	//���ñ���
	m_uButtonWidth=18;
	m_crEnableBack=RGB(255,255,255);
	m_crDisableBack=RGB(125,125,125);
	//������ɫ
	m_crEnableText=RGB(14,14,14);
	m_crDisableText=RGB(180,180,180);

	//�߿���ɫ
	m_crEnableBorad=RGB(102,161,191);
	m_crDisableBorad=RGB(170,170,170);//GetSysColor(COLOR_WINDOW);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CSkinComboBoxList::CSkinComboBoxList()
{
}

//��������
CSkinComboBoxList::~CSkinComboBoxList()
{
}

//�滭����
VOID CSkinComboBoxList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//��������
	CRect rcItem=lpDrawItemStruct->rcItem;
	CDC * pDCControl=CDC::FromHandle(lpDrawItemStruct->hDC);

	//��������
	CDC BufferDC;
	CBitmap ImageBuffer;
	BufferDC.CreateCompatibleDC(pDCControl);
	ImageBuffer.CreateCompatibleBitmap(pDCControl,rcItem.Width(),rcItem.Height());

	//���û���
	BufferDC.SelectObject(&ImageBuffer);
	BufferDC.SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//��ȡ�ַ�
	CString strString;
	GetText(lpDrawItemStruct->itemID,strString);

	//����λ��
	CRect rcString;
	rcString.SetRect(4,0,rcItem.Width()-8,rcItem.Height());

	//��ɫ����
	COLORREF crTextColor=((lpDrawItemStruct->itemState&ODS_SELECTED)!=0)?RGB(255,255,255):RGB(0,0,0);
	COLORREF crBackColor=((lpDrawItemStruct->itemState&ODS_SELECTED)!=0)?RGB(10,36,106):RGB(255,255,255);

	//�滭����
	BufferDC.FillSolidRect(0,0,rcItem.Width(),rcItem.Height(),crBackColor);

	//�滭�ַ�
	BufferDC.SetBkMode(TRANSPARENT);
	BufferDC.SetTextColor(crTextColor);
	BufferDC.DrawText(strString,&rcString,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	//�滭����
	pDCControl->BitBlt(rcItem.left,rcItem.top,rcItem.Width(),rcItem.Height(),&BufferDC,0,0,SRCCOPY);

	//������Դ
	BufferDC.DeleteDC();
	ImageBuffer.DeleteObject();

	return;
}

//����滭
VOID CSkinComboBoxList::OnNcPaint()
{
	__super::OnNcPaint();

	//��ȡλ��
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//�滭�߿�
	CWindowDC WindowDC(this);
	COLORREF crBoradFrame=CSkinComboBox::m_SkinAttribute.m_crEnableBorad;
	WindowDC.Draw3dRect(0,0,rcWindow.Width(),rcWindow.Height(),crBoradFrame,crBoradFrame);

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CSkinComboBox::CSkinComboBox()
{
	//���Ʊ���
	m_bHovering=false;
	m_bDrawBorad=true;
	m_bRenderImage=false;
	m_uButtonWidth=m_SkinAttribute.m_uButtonWidth;
	//������ɫ
	m_crEnableBack=m_SkinAttribute.m_crEnableBack;
	m_crDisableBack=m_SkinAttribute.m_crDisableBack;

	//������ɫ
	m_crEnableText=m_SkinAttribute.m_crEnableText;
	m_crDisableText=m_SkinAttribute.m_crDisableText;

	//�߿���ɫ
	m_crEnableBorad=m_SkinAttribute.m_crEnableBorad;
	m_crDisableBorad=m_SkinAttribute.m_crDisableBorad;
	//��Դ����
	m_uResourceID=0;
	m_hResInstance=NULL;

	return;
}

//��������
CSkinComboBox::~CSkinComboBox()
{
}

//�ؼ���
VOID CSkinComboBox::PreSubclassWindow()
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//���ÿؼ�
	Initialization();
	RectifyControl(rcClient.Width(),rcClient.Height());

	return;
}

//��������
VOID CSkinComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	//���ñ���
	lpMeasureItemStruct->itemWidth=0;
	lpMeasureItemStruct->itemHeight=GetItemHeight(LB_ERR);

	return;
}

//�滭����
VOID CSkinComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	if( lpDrawItemStruct->itemID == -1 )
		return;
}

//�滭�ؼ�
VOID CSkinComboBox::DrawControlView(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//״̬����
	DWORD dwStyle=GetStyle();
	bool bDisable=(IsWindowEnabled()==FALSE);
	bool bDropped=(GetDroppedState()!=FALSE);

	//�滭����
	DrawBackGround(pDC,bDisable);

	//�滭�߿�
	if (m_bDrawBorad==true)
	{
		if (bDisable==false)
		{
			if (m_bRenderImage==true)
			{
				//��������
				ASSERT(CSkinRenderManager::GetInstance()!=NULL);
				CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();

				//��Ⱦ��ɫ
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

	//����λ��
	INT nImageIndex=0;
	if (bDisable==true) nImageIndex=4;
	else if (bDropped==true) nImageIndex=1;
	else if (m_bHovering==true) nImageIndex=3;

	//���ذ�ť
	CBitImage ImageButton;
	if ((m_hResInstance==NULL)||(m_uResourceID==0))
	{
		ImageButton.LoadFromResource(GetModuleHandle(SKIN_CONTROLS_DLL_NAME),IDB_SKIN_COMBOBOX_BUTTON);
	}
	else
	{
		ImageButton.LoadFromResource(m_hResInstance,m_uResourceID);
	}

	//��Ⱦ��Դ
	CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();
	if ((pSkinRenderManager!=NULL)&&(m_bRenderImage==true)) pSkinRenderManager->RenderImage(ImageButton);

	//�滭��ť
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

	//�滭����
	if ((dwStyle & 0x000F)==CBS_DROPDOWNLIST)
	{
		//������ɫ
		if (GetFocus()->GetSafeHwnd()==m_hWnd)
		{
			pDC->FillSolidRect(0,0,rcClient.Width(),rcClient.Height(),m_crEnableBack);
		}
		else
		{
			pDC->FillSolidRect(0,0,rcClient.Width(),rcClient.Height(),m_crEnableBack);
		}
	
		//�滭�ַ�
		CString strString;
		GetWindowText(strString);

		//��ȡ�ַ�
		CRect rcText;
		GetClientRect(&rcText);

		//���û���
		pDC->SetBkMode(TRANSPARENT);
		pDC->SelectObject(GetFont());

		if (bDisable==false)
			pDC->SetTextColor(m_crEnableText);
		else
			pDC->SetTextColor(m_crDisableText);

		//�滭�ַ�
		rcText.left += 4;
		pDC->DrawText(strString,&rcText,DT_VCENTER|DT_SINGLELINE);
	}
}

//ʧȥ����
VOID CSkinComboBox::OnKillFocus(CWnd *pWnd)
{
	//���½���
	if (m_hWnd!=NULL) Invalidate(FALSE);
}

//��ť���
bool CSkinComboBox::VerdictOverButton(CPoint MousePoint)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//�����б�
	if ((rcClient.PtInRect(MousePoint)==TRUE)&&((GetStyle()&CBS_SIMPLE)!=0)) return true;

	//�������
	if ((MousePoint.y>(rcClient.Height()-1))||(MousePoint.y<1)) return false;
	if ((MousePoint.x<(rcClient.Width()-(INT)m_uButtonWidth-1))||(MousePoint.x>(rcClient.Width()-1))) return false;

	return true;
}

//�߿�����
VOID CSkinComboBox::SetDrawBorad(bool bDrawBorad)
{
	//���ñ���
	m_bDrawBorad=bDrawBorad;

	//���½���
	if (m_hWnd!=NULL) Invalidate(FALSE);

	return;
}

//������Ⱦ
VOID CSkinComboBox::SetRenderImage(bool bRenderImage)
{
	//���ñ���
	m_bRenderImage=bRenderImage;

	//���½���
	if (m_hWnd!=NULL) Invalidate(FALSE);

	return;
}

//������Դ
VOID CSkinComboBox::SetButtonImage(HINSTANCE hResInstance, UINT uResourceID, CSize & SizeButton)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//���ñ���
	m_uResourceID=uResourceID;
	m_hResInstance=hResInstance;

	//���ÿؼ�
	m_uButtonWidth=SizeButton.cx;
	SetItemHeight(LB_ERR,SizeButton.cy);

	//���ÿؼ�
	RectifyControl(rcClient.Width(),rcClient.Height());

	//���½���
	if (m_hWnd!=NULL) RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW);

	return;
}

//���ÿؼ�
VOID CSkinComboBox::Initialization()
{
	//����״̬
	m_bHovering=false;
	m_uButtonWidth=m_SkinAttribute.m_uButtonWidth;

	//��������
	COMBOBOXINFO ComboBoxInfo;
	ComboBoxInfo.cbSize=sizeof(ComboBoxInfo);

	//�󶨿ؼ�
	if (GetComboBoxInfo(&ComboBoxInfo)==TRUE)
	{
		if (ComboBoxInfo.hwndItem!=NULL) m_SkinComboBoxEdit.SubclassWindow(ComboBoxInfo.hwndItem);
		if (ComboBoxInfo.hwndList!=NULL) m_SkinComboBoxList.SubclassWindow(ComboBoxInfo.hwndList);
	}

	return;
}

//���°�ť
VOID CSkinComboBox::RedrawDropButton()
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��ťλ��
	CRect rcDropButton;
	rcDropButton.SetRect(rcClient.Width()-m_uButtonWidth-1,1,rcClient.Width()-1,rcClient.Height()-1);

	//���½���
	RedrawWindow(&rcDropButton,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW);

	return;
}

//�����ؼ�
VOID CSkinComboBox::RectifyControl(INT nWidth, INT nHeight)
{
	//�ƶ��ؼ�
	if (m_SkinComboBoxEdit.m_hWnd!=NULL)
	{
		//��ȡλ��
		CRect rcClient;
		GetClientRect(&rcClient);

		//�ƶ��ؼ�
		CSize SizeEdit;
		SizeEdit.SetSize(rcClient.Width()-m_uButtonWidth-10,12);
		m_SkinComboBoxEdit.SetWindowPos(NULL,2,(rcClient.Height()-12)/2,SizeEdit.cx,SizeEdit.cy,SWP_NOZORDER);
	}

	return;
}

//�ػ���Ϣ
VOID CSkinComboBox::OnPaint()
{
	CPaintDC dc(this);
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//���� DC
	CImageDC ImageDC(ImageBuffer);
	CDC * pBufferDC = CDC::FromHandle(ImageDC);

	//�滭����
	DrawControlView(pBufferDC);

	//�滭����
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return;
}

//�б�ر�
VOID CSkinComboBox::OnCbnCloseup()
{
	//���°�ť
	RedrawDropButton();

	//���½���
	if (m_hWnd!=NULL) Invalidate(FALSE);

	return;
}

//�����¼�
BOOL CSkinComboBox::OnCbnSelChange()
{
	//���½���
	if (m_hWnd!=NULL) Invalidate(FALSE);

	return 0;
}

//�滭����
BOOL CSkinComboBox::OnEraseBkgnd(CDC * pDC)
{
	return FALSE;
}

//ʱ����Ϣ
VOID CSkinComboBox::OnTimer(UINT_PTR nIDEvent)
{
	//�����ж�
	if (nIDEvent==IDI_CHECK_HOVER)
	{
		//�����ж�
		if (m_bHovering==true)
		{
			//��ȡ���
			CPoint MousePoint;
			GetCursorPos(&MousePoint);
			ScreenToClient(&MousePoint);

			//�����ж�
			m_bHovering=VerdictOverButton(MousePoint);

			//���½���
			if (m_bHovering==false) RedrawDropButton();
		}

		//ɾ��ʱ��
		if (m_bHovering==false) KillTimer(IDI_CHECK_HOVER);

		return;
	}

	__super::OnTimer(nIDEvent);
}

//λ����Ϣ
VOID CSkinComboBox::OnSize(UINT nType, INT cx, INT cy) 
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyControl(cx,cy);

	return;
}

//������Ϣ
INT CSkinComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//���ÿؼ�
	Initialization();
	RectifyControl(rcClient.Width(),rcClient.Height());

	return 0;
}

//������ɫ
VOID CSkinComboBox::SetEnableColor(COLORREF crEnableText, COLORREF crEnableBK, COLORREF crEnableBorad)
{
	//������ɫ
	m_crEnableBack=crEnableBK;
	m_crEnableText=crEnableText;
	m_crEnableBorad=crEnableBorad;

	m_SkinComboBoxEdit.SetEnableColor(crEnableText,crEnableBK,crEnableBorad);
	//���½���
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//������ɫ
VOID CSkinComboBox::SetDisableColor(COLORREF crDisableText, COLORREF crDisableBK, COLORREF crDisableBorad)
{
	//������ɫ
	m_crDisableBack=crDisableBK;
	m_crDisableText=crDisableText;
	m_crDisableBorad=crDisableBorad;

	m_SkinComboBoxEdit.SetDisableColor(crDisableText,crDisableBK,crDisableBorad);
	//���½���
	if (m_hWnd!=NULL)
	{
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_ERASENOW);
	}

	return;
}

//�����Ϣ
VOID CSkinComboBox::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);

	if (m_bHovering==false)
	{
		//�����ж�
		m_bHovering=VerdictOverButton(Point);

		//״̬�ж�
		if (m_bHovering==true)
		{
			//���½���
			RedrawDropButton();

			//����ʱ��
			SetTimer(IDI_CHECK_HOVER,TIME_CHECK_HOVER,NULL);
		}
	}

	return;
}

//�����Ϣ
VOID CSkinComboBox::OnLButtonDown(UINT nFlags, CPoint Point)
{
	//���ý���
	SetFocus();

	//��ʾ�б�
	if (VerdictOverButton(Point)==true)
	{
		//��ʾ�б�
		ShowDropDown(GetDroppedState()==FALSE);

		//���°�ť
		RedrawDropButton();
	}

	return;
}

//�����Ϣ
VOID CSkinComboBox::OnLButtonDblClk(UINT nFlags, CPoint Point)
{
	//���ý���
	SetFocus();

	//��ʾ�б�
	if (VerdictOverButton(Point)==true)
	{
		//��ʾ�б�
		ShowDropDown(GetDroppedState()==FALSE);

		//���°�ť
		RedrawDropButton();
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////