#ifndef DLG_INJECT_DIALOG_HEAD_FILE
#define DLG_INJECT_DIALOG_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "ProcessList.h"

//////////////////////////////////////////////////////////////////////////////////

//ע�ᴰ��
class CInjectDialog : public CSkinDialog
{
	//��������
public:
	tagProcessData*					m_pProcessData;
	//���ñ���
protected:

	//�ؼ�����
public:
	CSkinEdit						m_EditFile;
	CSkinEdit						m_EditLoadFunc;
	CSkinButton						m_ButtonFileBrowser;
	CSkinButton						m_ButtonOK;
	CSkinButton						m_ButtonCancel;

	//�������
protected:
	CSkinLayered					m_SkinLayered;						//�ֲ㴰��

	//��������
public:
	//���캯��
	CInjectDialog();
	//��������
	virtual ~CInjectDialog();
	void SetProcessData(tagProcessData *p) {
		m_pProcessData = p;
	}
	// �Ի�������
	enum { IDD = IDD_INJECT_DAILOG };
	//���غ���
public:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//���ú���
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
	//ȡ������
	virtual VOID OnCancel();

	//��Ϣӳ��
public:public:
	afx_msg void OnBnClickedButtonFileBrowser();
	afx_msg void OnBnClickedOk();

	//ϵͳ��Ϣ
protected:
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif