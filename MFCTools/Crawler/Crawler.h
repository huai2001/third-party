
// Crawler.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#include "../SkinControls/SkinControlsHead.h"

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

// CCrawlerApp:
// �йش����ʵ�֣������ Crawler.cpp
//

class CCrawlerApp : public CWinApp
{
	//��Դ���
public:
	CSkinRenderManager				m_SkinRenderManager;				//��Ⱦ����
	CSkinResourceManager			m_SkinResourceManager;				//��Դ����
public:
	CCrawlerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCrawlerApp theApp;