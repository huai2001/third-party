// ProcessModuleList.h : ͷ�ļ�
//

#pragma once

#include "Stdafx.h"
#include "Resource.h"

#define GUI_PROCESS_MODULE_LIST_ICON_ID			0
#define GUI_PROCESS_MODULE_LIST_ICON			_T("")
#define GUI_PROCESS_MODULE_LIST_ICON_SIZE		22

#define GUI_PROCESS_MODULE_LIST_NAME_ID			1
#define GUI_PROCESS_MODULE_LIST_NAME			_T("ģ����")
#define GUI_PROCESS_MODULE_LIST_NAME_SIZE		300

#define GUI_PROCESS_MODULE_LIST_ADDRESS_ID		2
#define GUI_PROCESS_MODULE_LIST_ADDRESS			_T("�����ַ")
#define GUI_PROCESS_MODULE_LIST_ADDRESS_SIZE	70

#define GUI_PROCESS_MODULE_LIST_BASESIZE_ID		3
#define GUI_PROCESS_MODULE_LIST_BASESIZE		_T("�����С")
#define GUI_PROCESS_MODULE_LIST_BASESIZE_SIZE	70


class CProcessModuleList : public CSkinListCtrl
{
	//�ؼ�����
protected:
	BOOL							m_bRectify;						//������־

	//��������
public:
	//���캯��
	CProcessModuleList();
	//��������
	virtual ~CProcessModuleList();

	//���غ���
protected:
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//���ݿ���
protected:
	//��������
	virtual INT SortItemData(LPARAM lParam1, LPARAM lParam2, word_t wColumnIndex, bool_t bAscendSort);
	//�����ַ�
	virtual VOID ConstructString(VOID * pItemData, WORD wColumnIndex, LPTSTR pszString, WORD wMaxCount);
	//��ȡλ��
	virtual INT GetInsertIndex(VOID * pItemData);
	//�滭����
protected:
	//�滭����
	virtual VOID DrawCustomItem(CDC * pDC, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect&rcSubItem, INT nColumnIndex);
	//���ܺ���
public:
	//��ʼ��
	void InitView();
// ʵ��
protected:
	/*//����б�
	afx_msg void OnLvnItemRightClickList(NMHDR *pNMHDR, LRESULT *pResult);
	//�һ��б�
	afx_msg void OnLvnItemLeftClickList(NMHDR *pNMHDR, LRESULT *pResult);*/
	//��С��Ϣ
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//����
	afx_msg VOID OnDestroy();

	DECLARE_MESSAGE_MAP()
};