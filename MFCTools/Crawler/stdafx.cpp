
// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// Crawler.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

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
	//ȡ��������
	HWND hDesktopWnd = ::GetDesktopWindow();

	//ȡ���������
	tagPOINT point;
	::GetCursorPos(&point);

	//ȡ�����ָ�봦���ھ��
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