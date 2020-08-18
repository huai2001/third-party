// ScreenshotWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Crawler.h"
#include "ScreenshotWnd.h"
#include "image.h"

#define LIMIT_BOX_MINIMUN 20

enum {
	BORDER_NULL = 0,
	BORDER_LEFT,
	BORDER_TOP,
	BORDER_RIGHT,
	BORDER_BOTTOM,
	BORDER_BOX_1,
	BORDER_BOX_2,
};
// CScreenshotWnd

IMPLEMENT_DYNAMIC(CScreenshotWnd, CWnd)


CScreenshotWnd::CScreenshotWnd()
{
	m_nIndex = BORDER_NULL;
	m_bLButtonDown = FALSE;
	m_cpDownPoint.SetPoint(0,0);
}

CScreenshotWnd::~CScreenshotWnd()
{
}

BEGIN_MESSAGE_MAP(CScreenshotWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


BOOL CScreenshotWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	return __super::PreCreateWindow(cs);
}

void CScreenshotWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bLButtonDown) {
		CRect rt(gCrawlerScreenshot.rcBox.left - 2, gCrawlerScreenshot.rcBox.top - 2, gCrawlerScreenshot.rcBox.right + 2, gCrawlerScreenshot.rcBox.bottom + 2);
		if (rt.PtInRect(point)) {
			//��
			if (point.x <= (rt.left + 4)) {
				m_nIndex = BORDER_LEFT;
				return;
			}
			//��
			if (point.x >= (rt.right - 4)) {
				m_nIndex = BORDER_RIGHT;
				return;
			}
			//��
			if (point.y <= rt.top + 4) {
				m_nIndex = BORDER_TOP;
				return;
			}
			//��
			if (point.y >= (rt.bottom - 4)) {
				m_nIndex = BORDER_BOTTOM;
				return;
			}

			INT nWidth = gCrawlerScreenshot.rcBox.left;

			INT nCount = (MAX_CRAWLER_DATA_BOX - 1);
			for(INT i = 0; i < nCount; i++) {
				nWidth += gCrawlerScreenshot.nBoxWidth[i];
				if (point.x > (nWidth - 4) && point.x < (nWidth + 4)) {
					m_nIndex = BORDER_BOX_1 + i;
					return ;
				}
			}
		}
		m_nIndex = BORDER_NULL;
		return ;
	}

	LONG w[MAX_CRAWLER_DATA_BOX];
	CRect rt(gCrawlerScreenshot.rcBox);
	for (INT i = 0; i < MAX_CRAWLER_DATA_BOX; i++) {
		w[i] = gCrawlerScreenshot.nBoxWidth[i];
	}

	switch (m_nIndex) {
	case BORDER_NULL:{
			CPoint lt(point.x - m_cpDownPoint.x,point.y - m_cpDownPoint.y);
			rt.MoveToXY(lt);
			break;
		}
		case BORDER_LEFT: {
			LONG limit = rt.right - w[1] - w[2] - LIMIT_BOX_MINIMUN;
			if (limit > point.x) {
				w[0] += (rt.left - point.x);
				rt.left = point.x;
			}
			break;
		}
		case BORDER_RIGHT: {
			LONG limit = rt.left + w[0] + w[1] + LIMIT_BOX_MINIMUN;
			if (limit < point.x ) {
				w[2] += (point.x - rt.right);
				rt.right = point.x;
			}
			break;
		}
		case BORDER_TOP:{
			if ((rt.bottom - 20) >= point.y) {
				rt.top = point.y;
			}
			break;
		}
		case BORDER_BOTTOM:{
			LONG limit = rt.top + LIMIT_BOX_MINIMUN;
			if (limit < point.y) {
				rt.bottom = point.y;
			}
			break;
		}
		case BORDER_BOX_1:{
			if ((rt.left + LIMIT_BOX_MINIMUN) < point.x && 
				(rt.left + w[0] + w[1] - LIMIT_BOX_MINIMUN) > point.x) {
				LONG width = (rt.left - point.x) + w[0];
				w[0] -= width;
				w[1] += width;
			}
			break;
		}
		case BORDER_BOX_2:{
			if ((rt.right - w[1] - w[2] + LIMIT_BOX_MINIMUN) < point.x && 
				(rt.right - LIMIT_BOX_MINIMUN) > point.x) {
					INT width = (rt.right - w[2] - point.x);
					w[1] -= width;
					w[2] += width;
			}
			break;
		}
	}
	gCrawlerScreenshot.rcBox = rt;
	for (INT i = 0; i < MAX_CRAWLER_DATA_BOX; i++) {
		gCrawlerScreenshot.nBoxWidth[i] = w[i];
	}
	Invalidate(TRUE);
}

void CScreenshotWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rt(gCrawlerScreenshot.rcBox.left - 2, gCrawlerScreenshot.rcBox.top - 2, gCrawlerScreenshot.rcBox.right + 2, gCrawlerScreenshot.rcBox.bottom + 2);
	if (rt.PtInRect(point)) {
		m_bLButtonDown = TRUE;
		m_cpDownPoint.SetPoint(point.x - gCrawlerScreenshot.rcBox.left, point.y - gCrawlerScreenshot.rcBox.top);
		Invalidate(TRUE);
		return;
	}

	m_bLButtonDown = FALSE;
}

void CScreenshotWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_bLButtonDown) {
		Invalidate(TRUE);
	}
	m_bLButtonDown = FALSE;
}
/*
IDC_APPSTARTING ��׼�ļ�ͷ��Сɳ©
IDC_ARROW ��׼�ļ�ͷ
IDC_CROSS ʮ�ֹ��
IDC_HELP ��׼�ļ�ͷ���ʺ�
IDC_IBEAM ���ֹ��
IDC_NO ��ֹȦ
IDC_SIZEALL �����ͷָ�򶫡������ϡ���
IDC_SIZENESW ˫��ͷָ�򶫱�������
IDC_SIZENS ˫��ͷָ���ϱ�
IDC_SIZENWSE ˫��ͷָ�������Ͷ���
IDC_SIZEWE ˫��ͷָ����
IDC_UPARROW ��ֱ��ͷ
IDC_WAIT ɳ©
*/
BOOL CScreenshotWnd::OnSetCursor(CWnd* pWnd,UINT nHitTest,UINT message)
{
	if (pWnd == this) {
		switch (m_nIndex) {
		case BORDER_LEFT:
		case BORDER_RIGHT: 
		case BORDER_BOX_1:
		case BORDER_BOX_2:
			SetCursor(LoadCursor(NULL,IDC_SIZEWE));
			return TRUE;
		case BORDER_TOP:
		case BORDER_BOTTOM:
			SetCursor(LoadCursor(NULL,IDC_SIZENS));
			return TRUE;
		}

		if (m_bLButtonDown && m_nIndex == BORDER_NULL) {
			SetCursor(LoadCursor(NULL,IDC_SIZEALL));
			return TRUE;
		}
	}
	return __super::OnSetCursor(pWnd, nHitTest, message);
}

int CScreenshotWnd::OnCreate(LPCREATESTRUCT lpcs)
{
	m_Font.CreateFont(18, 0, 0, 0, FW_NORMAL,
		FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,  DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, "΢���ź�");

	return __super::OnCreate(lpcs);
}

LRESULT CScreenshotWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_CLOSE) {
		INT nWidth = gCrawlerScreenshot.rcBox.right-gCrawlerScreenshot.rcBox.left;
		INT nHeight = gCrawlerScreenshot.rcBox.bottom-gCrawlerScreenshot.rcBox.top;
		_cc_image_t *src = m_Screenshot.toImage();
		_cc_image_t *dst = _cc_init_image(CF_A8R8G8B8, nWidth * 4, nHeight * 4);
		if (dst == NULL) {
			return NULL;
		}
		_cc_image_resampled(dst,src,0, 0, gCrawlerScreenshot.rcBox.left, gCrawlerScreenshot.rcBox.top, dst->width, dst->height, nWidth, nHeight);

		_cc_image_to_gray(dst);
		_cc_convert_bnary_bitmap(dst);
		_cc_write_JPG("./Screenshot.jpg", dst, 100);
		AfxGetMainWnd()->PostMessage(WM_MESSAGE_EVENT, EVENT_SETTING_CLOSE, 0);
	}
	return __super::WindowProc(message, wParam, lParam);
}

void CScreenshotWnd::DrawBox(CDC *dc)
{
	CPen bluePen;
	CBrush br;
	CRect rcWnd(gCrawlerScreenshot.rcBox);

	// ��������DC
	CDC MemDC;
	MemDC.CreateCompatibleDC(dc);

	LPBITMAPINFO pBitmapInfo = m_Screenshot.GetBitmapInfo();
	HBITMAP hMemBmp = CreateCompatibleBitmap(dc->GetSafeHdc(), pBitmapInfo->bmiHeader.biWidth, pBitmapInfo->bmiHeader.biHeight);
	HBITMAP hOldBitmap = NULL;
	// ������ʹ��hMemDC����
	SetDIBits(dc->GetSafeHdc(), hMemBmp, 0L, (DWORD)m_Screenshot.GetHeight(), m_Screenshot.GetImageData(), pBitmapInfo, (DWORD)DIB_RGB_COLORS);

	hOldBitmap = (HBITMAP)MemDC.SelectObject(hMemBmp);
	MemDC.SetBkMode(TRANSPARENT);

	dc->BitBlt(rcWnd.left, rcWnd.top, rcWnd.Width(), rcWnd.Height(), &MemDC, rcWnd.left, rcWnd.top, SRCCOPY);
	// �ͷ���Դ
	MemDC.SelectObject(hOldBitmap);
	::DeleteObject(hMemBmp);
	MemDC.DeleteDC();

	br.CreateStockObject(NULL_BRUSH);
	bluePen.CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
	CPen* pOldPen = dc->SelectObject(&bluePen);
	dc->SelectObject(&br);

	INT nWidth = 0;
	INT nCount = (MAX_CRAWLER_DATA_BOX - 1);
	for(INT i = 0; i < nCount; i++) {
		nWidth += gCrawlerScreenshot.nBoxWidth[i];
		dc->MoveTo(gCrawlerScreenshot.rcBox.left + nWidth, gCrawlerScreenshot.rcBox.top + 2);
		dc->LineTo(gCrawlerScreenshot.rcBox.left + nWidth, gCrawlerScreenshot.rcBox.bottom - 2);
	}

	dc->Rectangle(&gCrawlerScreenshot.rcBox);
	dc->SelectObject(pOldPen);
}

void AlphaBlendBitmap(HDC hDC, tagRECT& rect)
{
	// ����һ����hDC���ݵ��ڴ��豸�����Ļ���
	HDC hBuf = ::CreateCompatibleDC(hDC);
	// ����͸���Ȳ���
	BLENDFUNCTION ftn = { 0 };
	ftn.BlendOp = 0;
	ftn.BlendFlags = 0;
	ftn.SourceConstantAlpha = 100;  // ͸����ָ�� 
	ftn.AlphaFormat = 0;
	LONG nWidth = rect.right - rect.left;
	LONG nHeight = rect.bottom - rect.top;
	HBITMAP hMemBmp = CreateCompatibleBitmap(hDC, nWidth, nHeight);
	::SelectObject(hBuf, hMemBmp);
	::SetBkMode(hBuf,TRANSPARENT);
	::FillRect(hBuf, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
	// ָ��͸���Ȼ���
	::AlphaBlend(hDC, rect.left, rect.top, nWidth, nHeight, 
		hBuf, rect.left, rect.top, nWidth, nHeight, ftn);

	// �ͷ�λͼ
	::DeleteObject(hMemBmp);
	// �ͷż��ݵ��ڴ��豸�����Ļ���
	::DeleteDC(hBuf);
}

void CScreenshotWnd::OnPaint()
{
	// device context for painting
	CPaintDC dc(this);
	HDC hDC = dc.GetSafeHdc();
	// ��ȡ�ؼ�����
	CRect rcWnd;
	//CRect rcBox(gCrawlerScreenshot.rcBox);
	GetClientRect(&rcWnd);

	// ��������DC
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);

	LPBITMAPINFO pBitmapInfo = m_Screenshot.GetBitmapInfo();
	HBITMAP hMemBmp = CreateCompatibleBitmap(hDC, rcWnd.Width(), rcWnd.Height());
	HBITMAP hOldBitmap = (HBITMAP)MemDC.SelectObject(hMemBmp);
	MemDC.SetBkMode(TRANSPARENT);

	// ������ʹ��hMemDC����
	SetDIBits(hDC, hMemBmp, 0L, (DWORD)m_Screenshot.GetHeight(), m_Screenshot.GetImageData(), pBitmapInfo, (DWORD)DIB_RGB_COLORS);

	AlphaBlendBitmap(MemDC.GetSafeHdc(), rcWnd);
	
	// �����ڴ��е�ͼ��������Ļ����ʾ
	DrawBox(&MemDC);

	dc.BitBlt(rcWnd.left, rcWnd.top, rcWnd.Width(), rcWnd.Height(), &MemDC, 0, 0, SRCCOPY);
	// �ͷ���Դ
	MemDC.SelectObject(hOldBitmap);
	::DeleteObject(hMemBmp);
	MemDC.DeleteDC();
}

BOOL CScreenshotWnd::OnEraseBkgnd(CDC* pDC)
{
	//return __super::OnEraseBkgnd(pDC);
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
}

BOOL CScreenshotWnd::PreTranslateMessage(MSG* pMsg)
{
	return __super::PreTranslateMessage(pMsg);
}

// CScreenshotWnd ��Ϣ�������
void CScreenshotWnd::Open(HWND hWnd) {
	if (!m_Screenshot.Screenshot(hWnd)) {
		AfxMessageBox(_T("��ͼʧ�ܣ�"));
		return;
	}
	if (GetSafeHwnd() == NULL){
		CreateEx(0,
			AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW, LoadCursor (NULL, IDC_ARROW),(HBRUSH) GetStockObject (WHITE_BRUSH),AfxGetApp()->LoadIcon(IDR_MAINFRAME)),
			_T("��λ����"),
			WS_OVERLAPPEDWINDOW,
			0, 0, 860,600,//m_Screenshot.GetWidth(), m_Screenshot.GetHeight(),
			GetDesktopWindow()->GetSafeHwnd(),
			NULL,
			NULL);
	}

	CenterWindow();
	
	::SetWindowPos(GetSafeHwnd(),HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
	//
	Invalidate(TRUE);
	ShowWindow(SW_SHOW);
}