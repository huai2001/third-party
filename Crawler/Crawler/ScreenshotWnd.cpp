// ScreenshotWnd.cpp : 实现文件
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
	//  CREATESTRUCT cs 来修改窗口类或样式
	return __super::PreCreateWindow(cs);
}

void CScreenshotWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bLButtonDown) {
		CRect rt(gCrawlerScreenshot.rcBox.left - 2, gCrawlerScreenshot.rcBox.top - 2, gCrawlerScreenshot.rcBox.right + 2, gCrawlerScreenshot.rcBox.bottom + 2);
		if (rt.PtInRect(point)) {
			//左
			if (point.x <= (rt.left + 4)) {
				m_nIndex = BORDER_LEFT;
				return;
			}
			//右
			if (point.x >= (rt.right - 4)) {
				m_nIndex = BORDER_RIGHT;
				return;
			}
			//上
			if (point.y <= rt.top + 4) {
				m_nIndex = BORDER_TOP;
				return;
			}
			//下
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
IDC_APPSTARTING 标准的箭头和小沙漏
IDC_ARROW 标准的箭头
IDC_CROSS 十字光标
IDC_HELP 标准的箭头和问号
IDC_IBEAM 工字光标
IDC_NO 禁止圈
IDC_SIZEALL 四向箭头指向东、西、南、北
IDC_SIZENESW 双箭头指向东北和西南
IDC_SIZENS 双箭头指向南北
IDC_SIZENWSE 双箭头指向西北和东南
IDC_SIZEWE 双箭头指向东西
IDC_UPARROW 垂直箭头
IDC_WAIT 沙漏
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
		FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,  DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, "微软雅黑");

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

	// 创建兼容DC
	CDC MemDC;
	MemDC.CreateCompatibleDC(dc);

	LPBITMAPINFO pBitmapInfo = m_Screenshot.GetBitmapInfo();
	HBITMAP hMemBmp = CreateCompatibleBitmap(dc->GetSafeHdc(), pBitmapInfo->bmiHeader.biWidth, pBitmapInfo->bmiHeader.biHeight);
	HBITMAP hOldBitmap = NULL;
	// 在这里使用hMemDC绘制
	SetDIBits(dc->GetSafeHdc(), hMemBmp, 0L, (DWORD)m_Screenshot.GetHeight(), m_Screenshot.GetImageData(), pBitmapInfo, (DWORD)DIB_RGB_COLORS);

	hOldBitmap = (HBITMAP)MemDC.SelectObject(hMemBmp);
	MemDC.SetBkMode(TRANSPARENT);

	dc->BitBlt(rcWnd.left, rcWnd.top, rcWnd.Width(), rcWnd.Height(), &MemDC, rcWnd.left, rcWnd.top, SRCCOPY);
	// 释放资源
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
	// 创建一个与hDC兼容的内存设备上下文环境
	HDC hBuf = ::CreateCompatibleDC(hDC);
	// 设置透明度参数
	BLENDFUNCTION ftn = { 0 };
	ftn.BlendOp = 0;
	ftn.BlendFlags = 0;
	ftn.SourceConstantAlpha = 100;  // 透明度指定 
	ftn.AlphaFormat = 0;
	LONG nWidth = rect.right - rect.left;
	LONG nHeight = rect.bottom - rect.top;
	HBITMAP hMemBmp = CreateCompatibleBitmap(hDC, nWidth, nHeight);
	::SelectObject(hBuf, hMemBmp);
	::SetBkMode(hBuf,TRANSPARENT);
	::FillRect(hBuf, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
	// 指定透明度绘制
	::AlphaBlend(hDC, rect.left, rect.top, nWidth, nHeight, 
		hBuf, rect.left, rect.top, nWidth, nHeight, ftn);

	// 释放位图
	::DeleteObject(hMemBmp);
	// 释放兼容的内存设备上下文环境
	::DeleteDC(hBuf);
}

void CScreenshotWnd::OnPaint()
{
	// device context for painting
	CPaintDC dc(this);
	HDC hDC = dc.GetSafeHdc();
	// 获取控件矩形
	CRect rcWnd;
	//CRect rcBox(gCrawlerScreenshot.rcBox);
	GetClientRect(&rcWnd);

	// 创建兼容DC
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);

	LPBITMAPINFO pBitmapInfo = m_Screenshot.GetBitmapInfo();
	HBITMAP hMemBmp = CreateCompatibleBitmap(hDC, rcWnd.Width(), rcWnd.Height());
	HBITMAP hOldBitmap = (HBITMAP)MemDC.SelectObject(hMemBmp);
	MemDC.SetBkMode(TRANSPARENT);

	// 在这里使用hMemDC绘制
	SetDIBits(hDC, hMemBmp, 0L, (DWORD)m_Screenshot.GetHeight(), m_Screenshot.GetImageData(), pBitmapInfo, (DWORD)DIB_RGB_COLORS);

	AlphaBlendBitmap(MemDC.GetSafeHdc(), rcWnd);
	
	// 将在内存中的图拷贝到屏幕上显示
	DrawBox(&MemDC);

	dc.BitBlt(rcWnd.left, rcWnd.top, rcWnd.Width(), rcWnd.Height(), &MemDC, 0, 0, SRCCOPY);
	// 释放资源
	MemDC.SelectObject(hOldBitmap);
	::DeleteObject(hMemBmp);
	MemDC.DeleteDC();
}

BOOL CScreenshotWnd::OnEraseBkgnd(CDC* pDC)
{
	//return __super::OnEraseBkgnd(pDC);
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
}

BOOL CScreenshotWnd::PreTranslateMessage(MSG* pMsg)
{
	return __super::PreTranslateMessage(pMsg);
}

// CScreenshotWnd 消息处理程序
void CScreenshotWnd::Open(HWND hWnd) {
	if (!m_Screenshot.Screenshot(hWnd)) {
		AfxMessageBox(_T("截图失败！"));
		return;
	}
	if (GetSafeHwnd() == NULL){
		CreateEx(0,
			AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW, LoadCursor (NULL, IDC_ARROW),(HBRUSH) GetStockObject (WHITE_BRUSH),AfxGetApp()->LoadIcon(IDR_MAINFRAME)),
			_T("定位数据"),
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