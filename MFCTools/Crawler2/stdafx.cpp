
// stdafx.cpp : 只包括标准包含文件的源文件
// Crawler.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "winmm")

#ifndef _DEBUG
	#ifndef _UNICODE
		#pragma comment (lib,"libcc_vs2010.lib")
		#pragma comment (lib,"./Release/SkinControls.lib")
	#else
		#pragma comment (lib,"libccu_vs2010.lib")
		#pragma comment (lib,"./Release/SkinControls.lib")
	#endif
#else
	#ifndef _UNICODE
		#pragma comment (lib,"libcc_vs2010d.lib")
		#pragma comment (lib,"libcc_memory_vs2010.lib")
		#pragma comment (lib,"./Debug/SkinControlsD.lib")
	#else
		#pragma comment (lib,"libccud_vs2010.lib")
		#pragma comment (lib,"./Debug/SkinControlsD.lib")
	#endif
#endif

#if defined _M_IX86
    #pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
    #pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
    #pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
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