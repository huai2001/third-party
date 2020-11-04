// CrawlerDialog.cpp : 实现文件
//

#include "StdAfx.h"
#include "Screenshots.h"


CScreenshots::CScreenshots(void)
{
	//m_lpDirectDraw = NULL;
	m_cbImageBuffer = NULL;
	m_cbBuffer = NULL;
	m_dwBufferLen = 0;
	m_dwImageBufferLen = 0;
	ZeroMemory(&m_BitmapInfo, sizeof(m_BitmapInfo));
}

CScreenshots::~CScreenshots(void)
{
	if (m_cbBuffer) {
		free(m_cbBuffer);
		m_cbBuffer = NULL;
	}
	m_dwBufferLen = NULL;

	if (m_cbImageBuffer) {
		free(m_cbImageBuffer);
		m_cbImageBuffer = NULL;
	}
	m_dwImageBufferLen = 0;
}

/*
BOOL CScreenshots::DXScreenshost(RECT rect)
{
	HRESULT hr;
	DDSURFACEDESC DDSdesc;
	LPDIRECTDRAWSURFACE lpDDSPrime = NULL;
	LPDIRECTDRAWSURFACE lpDDSBack = NULL;
	LPDIRECTDRAWSURFACE lpDDSGdi = NULL;

	BOOL result = FALSE;
	DWORD dwWidth;
	DWORD dwHeight;

	if (m_lpDirectDraw == NULL) {
		hr = DirectDrawCreate(0, &m_lpDirectDraw, 0);
		if (FAILED(hr)) {
			return FALSE;
		}

		hr = m_lpDirectDraw->SetCooperativeLevel(NULL, DDSCL_NORMAL);
		if (FAILED(hr)) {
			m_lpDirectDraw->Release();
			m_lpDirectDraw = NULL;
			return FALSE;
		}
	}

	ZeroMemory(&DDSdesc, sizeof(DDSdesc));
	DDSdesc.dwSize = sizeof(DDSdesc);
	DDSdesc.dwFlags = DDSD_CAPS;
	DDSdesc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	hr = m_lpDirectDraw->CreateSurface(&DDSdesc, &lpDDSPrime, 0);
	if (FAILED(hr)) {
		goto DX_SCREENSHOTS_FAIL;
	}
	hr = m_lpDirectDraw->GetGDISurface(&lpDDSGdi);
	if (FAILED(hr)) {
		goto DX_SCREENSHOTS_FAIL;
	}

	ZeroMemory(&DDSdesc, sizeof(DDSdesc));
	DDSdesc.dwSize = sizeof(DDSdesc);
	DDSdesc.dwFlags = DDSD_ALL;
	hr = lpDDSPrime->GetSurfaceDesc(&DDSdesc);
	if (FAILED(hr)) {
		goto DX_SCREENSHOTS_FAIL;
	}

	// 初始化位图信息
	if ((DDSdesc.dwFlags & DDSD_WIDTH) && (DDSdesc.dwFlags & DDSD_HEIGHT)) {
		dwWidth = DDSdesc.dwWidth;
		dwHeight = DDSdesc.dwHeight;
	} else{
		goto DX_SCREENSHOTS_FAIL;
	}

	m_BitmapInfo.bmiHeader.biCompression = BI_RGB;//BI_BITFIELDS;
	m_BitmapInfo.bmiHeader.biBitCount = (WORD)DDSdesc.ddpfPixelFormat.dwRGBBitCount;

	//biBitCount = DDSdesc.ddpfPixelFormat.dwFlags & DDPF_RGB;

	ZeroMemory(&DDSdesc, sizeof(DDSdesc));
	DDSdesc.dwSize = sizeof(DDSdesc);
	DDSdesc.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH; 
	DDSdesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	DDSdesc.dwHeight = dwWidth;
	DDSdesc.dwWidth  = dwHeight;
	hr = m_lpDirectDraw->CreateSurface(&DDSdesc, &lpDDSBack, 0);
	if (FAILED(hr)) {
		goto DX_SCREENSHOTS_FAIL;
	}
	m_BitmapInfo.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
	m_BitmapInfo.bmiHeader.biWidth = rect.right - rect.left;
	m_BitmapInfo.bmiHeader.biHeight = rect.bottom - rect.top;
	m_BitmapInfo.bmiHeader.biPlanes = 1;
	m_BitmapInfo.bmiHeader.biCompression = BI_RGB;
	m_BitmapInfo.bmiHeader.biSizeImage = abs((m_BitmapInfo.bmiHeader.biWidth * m_BitmapInfo.bmiHeader.biHeight * m_BitmapInfo.bmiHeader.biBitCount) / 8);
	m_BitmapInfo.bmiHeader.biClrUsed  = 0;
	m_BitmapInfo.bmiHeader.biClrImportant = 0;

	if (m_cbImageBuffer == NULL || m_dwImageBufferLen < m_BitmapInfo.bmiHeader.biSizeImage) {
		if (m_cbImageBuffer) {
			free(m_cbImageBuffer);
			m_cbImageBuffer = NULL;
		}
		m_cbImageBuffer = (BYTE*)malloc(m_BitmapInfo.bmiHeader.biSizeImage);
		m_dwImageBufferLen = m_BitmapInfo.bmiHeader.biSizeImage;
	}

	if (m_cbImageBuffer == NULL) {
		goto DX_SCREENSHOTS_FAIL;
	}

	hr = lpDDSBack->BltFast(rect.left,rect.top,lpDDSPrime,&rect,DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);
	if (FAILED(hr)) {
		goto DX_SCREENSHOTS_FAIL;
	}

	ZeroMemory(&DDSdesc, sizeof(DDSdesc));
	DDSdesc.dwSize = sizeof(DDSdesc);

	hr   = lpDDSBack->Lock(&rect, &DDSdesc, DDLOCK_READONLY | DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);
	if (FAILED(hr)){
		goto DX_SCREENSHOTS_FAIL;
	}

	// copy the data into our buffer
	memcpy(m_cbImageBuffer, DDSdesc.lpSurface, m_BitmapInfo.bmiHeader.biSizeImage);

	lpDDSBack->Unlock(DDSdesc.lpSurface);

	result = TRUE;

DX_SCREENSHOTS_FAIL:
	if(lpDDSPrime)
		lpDDSPrime->Release();
	if(lpDDSGdi)
		lpDDSGdi->Release();
	if(lpDDSBack)
		lpDDSBack->Release();

	return result;
}*/

BOOL CScreenshots::Screenshot(HWND hWnd)
{
	HDC hDC = NULL;
	HDC hMemDC = NULL;
	INT nWidth;
	INT nHeight;
	BOOL result = FALSE;

	RECT rect = { 0 };
	if (hWnd == NULL) {
		hDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
		nWidth = GetDeviceCaps(hDC, HORZRES);
		nHeight = GetDeviceCaps(hDC, VERTRES);
		rect.right = nWidth;
		rect.bottom = nHeight;
	} else {
		GetWindowRect(hWnd, &rect);
		nWidth = rect.right - rect.left;
		nHeight = rect.bottom - rect.top;
		hDC = ::GetWindowDC(hWnd);
	}
	/*
	if (DXScreenshost(rect)) {
		return TRUE;
	}*/
	
	hMemDC = CreateCompatibleDC(hDC);
	HBITMAP hBmpScreen = CreateCompatibleBitmap(hDC, nWidth, nHeight);
	HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, hBmpScreen);

	BITMAP bm;
	GetObject(hBmpScreen, sizeof(bm), &bm);

	// 图片的像素数据
	ZeroMemory(&m_BitmapInfo, sizeof(BITMAPINFOHEADER));
	m_BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_BitmapInfo.bmiHeader.biWidth = bm.bmWidth;
	m_BitmapInfo.bmiHeader.biHeight = bm.bmHeight;
	m_BitmapInfo.bmiHeader.biPlanes = bm.bmPlanes;
	m_BitmapInfo.bmiHeader.biBitCount = bm.bmBitsPixel;
	m_BitmapInfo.bmiHeader.biCompression = BI_RGB;
	m_BitmapInfo.bmiHeader.biSizeImage = bm.bmHeight * bm.bmWidthBytes;

	if (m_cbImageBuffer == NULL || m_dwImageBufferLen < m_BitmapInfo.bmiHeader.biSizeImage) {
		if (m_cbImageBuffer) {
			free(m_cbImageBuffer);
			m_cbImageBuffer = NULL;
		}
		m_cbImageBuffer = (BYTE*)malloc(m_BitmapInfo.bmiHeader.biSizeImage);
		m_dwImageBufferLen = m_BitmapInfo.bmiHeader.biSizeImage;
	}

	if (m_cbImageBuffer == NULL) {
		goto DC_SCREENSHOTS_FAIL;
	}

	if (hWnd == NULL) {
		BitBlt(hMemDC, 0, 0, nWidth, nHeight, hDC, 0, 0, SRCCOPY);
	} else {
		if (!::PrintWindow(hWnd, hMemDC, PW_CLIENTONLY )) {
			::ShowWindow(hWnd,SW_SHOW);
			::UpdateWindow(hWnd);
			::SetWindowPos(hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOACTIVATE);
			BitBlt(hMemDC, rect.left, rect.top, nWidth, nHeight, hDC, 0, 0, SRCCOPY);
		}
	}

	GetDIBits(hMemDC, hBmpScreen, 0L, (DWORD)nHeight, m_cbImageBuffer, (LPBITMAPINFO)&m_BitmapInfo, (DWORD)DIB_RGB_COLORS);

	hBmpScreen = (HBITMAP)SelectObject(hMemDC, hOldBmp);
	result = TRUE;

DC_SCREENSHOTS_FAIL:
	DeleteDC(hMemDC);
	DeleteObject(hBmpScreen);
	DeleteObject(hOldBmp);
	ReleaseDC(hWnd,hDC);
	return result;
}

void CScreenshots::Save(LPTSTR lpFileName)
{
	BITMAPFILEHEADER bfh = { 0 };
	bfh.bfType = 0x4D42;  // "BM"((WORD)('M' << 8) | 'B');
	bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + m_BitmapInfo.bmiHeader.biSizeImage;
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	HANDLE hFile = CreateFile(lpFileName, GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	DWORD dwWrite;
	WriteFile(hFile, &bfh, sizeof(BITMAPFILEHEADER), &dwWrite, NULL);
	WriteFile(hFile, &m_BitmapInfo.bmiHeader, sizeof(BITMAPINFOHEADER), &dwWrite, NULL);
	WriteFile(hFile, m_cbImageBuffer, m_BitmapInfo.bmiHeader.biSizeImage, &dwWrite, NULL);
	CloseHandle(hFile);
}

_cc_image_t* CScreenshots::toImage() {
	_cc_image_t* img = NULL;
	_cc_int32_t len = 0;

	DWORD dwBufferLen = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + m_BitmapInfo.bmiHeader.biSizeImage;
	BITMAPFILEHEADER *bfh;

	if (dwBufferLen > m_dwBufferLen) {
		if (m_cbBuffer) {
			free(m_cbBuffer);
		}
		m_cbBuffer = (BYTE*)malloc(dwBufferLen);
		m_dwBufferLen = dwBufferLen;
	}

	if (m_cbBuffer == NULL) {
		TCHAR _TempPath[_CC_MAX_PATH_];
		TCHAR _FilePath[_CC_MAX_PATH_];
		GetTempPath(_CC_MAX_PATH_, _TempPath);
		_sntprintf_s(_FilePath, _cc_countof(_FilePath), _T("%s\\screenshots.bmp"), _TempPath);
		Save(_FilePath);
		return _cc_load_image(_FilePath);
	}

	bfh = (PBITMAPFILEHEADER)m_cbBuffer;
	bfh->bfType = 0x4D42;  // "BM"((WORD)('M' << 8) | 'B');
	bfh->bfSize = dwBufferLen;
	bfh->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	/*
	wBitsPixel = m_BitmapInfo.bmiHeader.biBitCount;
	m_BitmapInfo.bmiHeader.biBitCount = 24;*/

	len += sizeof(BITMAPFILEHEADER);
	CopyMemory(m_cbBuffer + len,  &m_BitmapInfo.bmiHeader, sizeof(BITMAPINFOHEADER));
	len += sizeof(BITMAPINFOHEADER);
	CopyMemory(m_cbBuffer + len, m_cbImageBuffer, m_BitmapInfo.bmiHeader.biSizeImage);
	len += m_BitmapInfo.bmiHeader.biSizeImage;

	//m_BitmapInfo.bmiHeader.biBitCount = wBitsPixel;

	return _cc_load_BMP(m_cbBuffer, len);
}