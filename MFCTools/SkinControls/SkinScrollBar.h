#ifndef SKIN_SCROLL_BAR_HEAD_FILE
#define SKIN_SCROLL_BAR_HEAD_FILE

#pragma once

#include "SkinControlsHead.h"
#include "SkinRenderManager.h"
#include "SkinResourceManager.h"

//////////////////////////////////////////////////////////////////////////////////

//������Դ
class SKIN_CONTROLS_CLASS CSkinScrollBarAttribute
{
	//��Դ����
public:
	CBitImage						m_ImageScroll;						//������Դ

	//��������
public:
	//���캯��
	CSkinScrollBarAttribute();
	//��������
	virtual ~CSkinScrollBarAttribute();

	//������
public:
	//������Դ
	bool Initialization(CSkinRenderManager * pSkinRenderManager);
};

//////////////////////////////////////////////////////////////////////////////////

//��������
class SKIN_CONTROLS_CLASS CSkinScrollBar
{
	//��Դ����
public:
	static CSkinScrollBarAttribute	m_SkinAttribute;					//��Դ����
public:
	CWnd * m_pWnd;
	//��������
public:
	//���캯��
	CSkinScrollBar();
	//��������
	virtual ~CSkinScrollBar();

	//���ܺ���
public:
	//���ù���
	VOID InitScroolBar(CWnd * pWnd);
	//���ù���
	VOID InitScroolBar(CWnd * pWnd, CBitImage &BitImage);
	//��ʾ����
	void ShowScrollBar(BOOL bShow);
};

//////////////////////////////////////////////////////////////////////////////////

#endif