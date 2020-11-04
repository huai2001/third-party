#ifndef SKIN_COMBOBOX_HEAD_FILE
#define SKIN_COMBOBOX_HEAD_FILE

#pragma once

#include "SkinEdit.h"
#include "SkinControlsHead.h"
#include "SkinRenderManager.h"
#include "SkinResourceManager.h"

//////////////////////////////////////////////////////////////////////////////////

//��ť����
class SKIN_CONTROLS_CLASS CSkinComboBoxAttribute
{
	//���ò���
public:
	UINT							m_uButtonWidth;						//��ť���
	COLORREF						m_crEnableBack;						//������ɫ
	COLORREF						m_crDisableBack;					//������ɫ
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
	CSkinComboBoxAttribute();
	//��������
	virtual ~CSkinComboBoxAttribute();

	//������
public:
	//������Դ
	bool Initialization(CSkinRenderManager * pSkinRenderManager);
};

//////////////////////////////////////////////////////////////////////////////////

//�б�ؼ�
class SKIN_CONTROLS_CLASS CSkinComboBoxList : public CListBox
{
	//��������
public:
	//���캯��
	CSkinComboBoxList();
	//��������
	virtual ~CSkinComboBoxList();

	//���غ���
protected:
	//�滭����
	virtual VOID DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct); 

	//��Ϣӳ��
protected:
	//����滭
	VOID OnNcPaint();

    DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//ѡ��ؼ�
class SKIN_CONTROLS_CLASS CSkinComboBox : public CComboBox
{
	//״̬����
protected:
	bool							m_bHovering;						//������־
	bool							m_bDrawBorad;						//�滭�߿�
	bool							m_bRenderImage;						//��Ⱦ��־
	UINT							m_uButtonWidth;						//��ť���
	//������ɫ
public:
	COLORREF						m_crEnableBack;						//������ɫ
	COLORREF						m_crDisableBack;					//������ɫ

	//������ɫ
public:
	COLORREF						m_crEnableText;						//������ɫ
	COLORREF						m_crDisableText;					//������ɫ

	//�߿���ɫ
public:
	COLORREF						m_crEnableBorad;					//�߿���ɫ
	COLORREF						m_crDisableBorad;					//�߿���ɫ
	//��Դ����
protected:
	UINT							m_uResourceID;						//��Դ��ʶ
	HINSTANCE						m_hResInstance;						//��Դ���

	//�ؼ�����
protected:
	CSkinEdit						m_SkinComboBoxEdit;					//�༭�ؼ�
	CSkinComboBoxList				m_SkinComboBoxList;					//�б�ؼ�

	//��ť����
public:
	static CSkinComboBoxAttribute	m_SkinAttribute;					//��ť����

	//��������
public:
	//���캯��
	CSkinComboBox();
	//��������
	virtual ~CSkinComboBox();

	//���غ���
protected:
	//�ؼ���
	virtual VOID PreSubclassWindow();
	//�滭����
	virtual VOID DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//��������
	virtual VOID MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	//���ܺ���
public:
	//�߿�����
	VOID SetDrawBorad(bool bDrawBorad);
	//������Ⱦ
	VOID SetRenderImage(bool bRenderImage);
	//������Դ
	VOID SetButtonImage(HINSTANCE hResInstance, UINT uResourceID, CSize & SizeButton);
	//���ܺ���
public:
	//������ɫ
	VOID SetEnableColor(COLORREF crEnableText, COLORREF crEnableBK, COLORREF crEnableBorad);
	//������ɫ
	VOID SetDisableColor(COLORREF crDisableText, COLORREF crDisableBK, COLORREF crDisableBorad);

	//��������
protected:
	//���Ʊ���
	virtual void DrawBackGround(CDC *pDC,bool bDisable);
	//�滭�ؼ�
	virtual VOID DrawControlView(CDC * pDC);
	//��ť���
	bool VerdictOverButton(CPoint MousePoint);

	//�ڲ�����
private:
	//���ÿؼ�
	VOID Initialization();
	//���°�ť
	VOID RedrawDropButton();
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);

	//��Ϣӳ��
protected:
	//�ػ���Ϣ
    VOID OnPaint();
	//�б�ر�
	VOID OnCbnCloseup();
	//�����¼�
	BOOL OnCbnSelChange();
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//ʱ����Ϣ
	VOID OnTimer(UINT_PTR nIDEvent);
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//����ƶ�
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonDblClk(UINT nFlags, CPoint Point);
	//ʧȥ����
	VOID OnKillFocus(CWnd *pWnd);
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif