
// stdafx.cpp : 只包括标准包含文件的源文件
// Crawler.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"
#pragma comment(lib, "ddraw.lib")
#pragma comment(lib, "psapi.lib")

#ifndef _DEBUG
	#ifndef _UNICODE
		#pragma comment (lib,"libcc_vs2010.lib")
		#pragma comment (lib,"../Release/SkinControls.lib")
	#else
		#pragma comment (lib,"libccu_vs2010.lib")
		#pragma comment (lib,"../Release/SkinControls.lib")
	#endif
#else
	#ifndef _UNICODE
		#pragma comment (lib,"libccd_vs2010.lib")
		#pragma comment (lib,"../Debug/SkinControlsD.lib")
	#else
		#pragma comment (lib,"libccud_vs2010.lib")
		#pragma comment (lib,"../Debug/SkinControlsD.lib")
	#endif
#endif

HWND GetWindowFromPoint() {
	HWND hFromWnd = NULL;
	//取得桌面句柄
	HWND hDesktopWnd = ::GetDesktopWindow();

	//取得鼠标坐标
	tagPOINT point;
	::GetCursorPos(&point);

	//取得鼠标指针处窗口句柄
	hFromWnd = ::WindowFromPoint(point);
	/*
	HWND grayHwnd = ::GetWindow(hFromWnd, GW_CHILD);
	RECT tempRc;
	BOOL bFind = FALSE;

	while (grayHwnd) {
		::GetWindowRect(grayHwnd, &tempRc);
		if(::PtInRect(&tempRc, pnt)) {
			bFind = TRUE;
			break;
		} else{
			grayHwnd = ::GetWindow(grayHwnd, GW_HWNDNEXT);
		}

	}
	if(bFind == TRUE) {
		hFromWnd = grayHwnd;
	}
	*/
	return hFromWnd;
}