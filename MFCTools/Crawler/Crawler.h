
// Crawler.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#include "../SkinControls/SkinControlsHead.h"

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

// CCrawlerApp:
// 有关此类的实现，请参阅 Crawler.cpp
//

class CCrawlerApp : public CWinApp
{
	//资源组件
public:
	CSkinRenderManager				m_SkinRenderManager;				//渲染管理
	CSkinResourceManager			m_SkinResourceManager;				//资源管理
public:
	CCrawlerApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CCrawlerApp theApp;