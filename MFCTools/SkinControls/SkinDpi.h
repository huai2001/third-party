
#ifndef SKIN_SKINDPI_HEAD_FILE
#define SKIN_SKINDPI_HEAD_FILE

#pragma once

#include "SkinControlsHead.h"

//////////////////////////////////////////////////////////////////////////////////
class SKIN_CONTROLS_CLASS CSkintDPI
{
	//��������
protected:
    HINSTANCE                            m_Inst;
    HWND                                 m_Hwnd;
    HFONT                                m_Font;
	HFONT                                m_Oldfont;
	LPCTSTR                              m_lpszTemplateName;

	//��������
public:
	//���캯��
    CSkintDPI();
	//��������
	virtual ~CSkintDPI();

	//���ܺ���
public:
	//���Ӻ���
    void Attach(HINSTANCE inst,HWND dlg,LPCTSTR lpszTemplateName,double dpi);
	//���Ӻ���
	void Attach(HINSTANCE inst,HWND dlg,UINT IDD,double dpi);
	//���뺯��
    void Detach();
};

#endif
