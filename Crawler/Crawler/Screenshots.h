
// Screenshots.h : Í·ÎÄ¼þ
//
#include <ddraw.h>

#include "image.h"

#pragma once

class CScreenshots
{
public:
	BYTE*	m_cbImageBuffer;
	DWORD	m_dwImageBufferLen;

	BYTE*	m_cbBuffer;
	DWORD	m_dwBufferLen;

	BITMAPINFO		m_BitmapInfo;
	//LPDIRECTDRAW	m_lpDirectDraw;

public:
	CScreenshots(void);
	virtual ~CScreenshots(void);

public:
	INT GetWidth() {
		return m_BitmapInfo.bmiHeader.biWidth;
	}

	INT GetHeight() {
		return m_BitmapInfo.bmiHeader.biHeight;
	}

	LPBITMAPINFO GetBitmapInfo() {
		return &m_BitmapInfo;
	}

	PBYTE GetImageData() {
		return m_cbImageBuffer;
	}

protected:
	//BOOL DXScreenshost(RECT rect);
public:
	BOOL Screenshot(HWND hWnd);

	void Save(LPTSTR lpFileName);
	_cc_image_t *toImage();
};

