#ifndef SKIN_EDIT_HEAD_FILE
#define SKIN_EDIT_HEAD_FILE

#pragma once

#include "SkinControlsHead.h"
#include "SkinRenderManager.h"

//////////////////////////////////////////////////////////////////////////////////

//�ؼ�����
class SKIN_CONTROLS_CLASS CSkinEditAttribute
{
	//������ɫ
public:
	COLORREF						m_crEnableBK;						//������ɫ
	COLORREF						m_crDisableBK;						//������ɫ

	//������ɫ
public:
	COLORREF						m_crEnableText;						//������ɫ
	COLORREF						m_crDisableText;					//������ɫ

	//�߿���ɫ
public:
	COLORREF						m_crEnableBorad;					//�߿���ɫ
	COLORREF						m_crDisableBorad;					//�߿���ɫ

	//��������
public:
	//���캯��
	CSkinEditAttribute();
	//��������
	virtual ~CSkinEditAttribute();

	//������
public:
	//������Դ
	bool Initialization(CSkinRenderManager * pSkinRenderManager);
};

//////////////////////////////////////////////////////////////////////////////////

//�༭��
class SKIN_CONTROLS_CLASS CSkinEdit : public CEdit
{
	//��̬����
public:
	static CSkinEditAttribute		m_SkinAttribute;					//��������

	//��ˢ����
public:
	CBrush							m_brEnable;							//��ˢ��Դ
	CBrush							m_brDisable;						//��ˢ��Դ

	//������ɫ
public:
	COLORREF						m_crEnableBK;						//������ɫ
	COLORREF						m_crDisableBK;						//������ɫ

	//������ɫ
public:
	COLORREF						m_crEnableText;						//������ɫ
	COLORREF						m_crDisableText;					//������ɫ

	//�߿���ɫ
public:
	COLORREF						m_crEnableBorad;					//�߿���ɫ
	COLORREF						m_crDisableBorad;					//�߿���ɫ

	//��������
public:
	//���캯��
	CSkinEdit();
	//��������
	virtual ~CSkinEdit();

	//���ܺ���
public:
	//������ɫ
	VOID SetEnableColor(COLORREF crEnableText, COLORREF crEnableBK, COLORREF crEnableBorad);
	//������ɫ
	VOID SetDisableColor(COLORREF crDisableText, COLORREF crDisableBK, COLORREF crDisableBorad);

	//��Ϣ���� 
protected:
	//�滭�ؼ�
	VOID DrawControlView(CDC * pDC, COLORREF crColorBorad,  COLORREF crColorBK);
	//�ػ���Ϣ
    VOID OnNcPaint();
	//�滭�ؼ�
	HBRUSH CtlColor(CDC * pDC, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif