// ProcessList.h : ͷ�ļ�
//

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#define GUI_PROCESS_LIST_ICON_ID			0
#define GUI_PROCESS_LIST_ICON				_T("")
#define GUI_PROCESS_LIST_ICON_SIZE			22

#define GUI_PROCESS_LIST_NAME_ID			1
#define GUI_PROCESS_LIST_NAME				_T("������")
#define GUI_PROCESS_LIST_NAME_SIZE			300

#define GUI_PROCESS_LIST_PID_ID				2
#define GUI_PROCESS_LIST_PID				_T("����ID")
#define GUI_PROCESS_LIST_PID_SIZE			70

#define GUI_PROCESS_LIST_THREADS_ID			3
#define GUI_PROCESS_LIST_THREADS			_T("�߳���")
#define GUI_PROCESS_LIST_THREADS_SIZE		70

typedef struct tagProcessData {
	TCHAR* szModule;
	DWORD dwProcessID;
	DWORD dwThreadCount;
	HICON hIcon;
}tagProcessData;


class CProcessList : public CSkinListCtrl
{
	//�ؼ�����
protected:
	BOOL							m_bRectify;						//������־

	//��������
public:
	//���캯��
	CProcessList();
	//��������
	virtual ~CProcessList();

	//���غ���
protected:
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//ɾ��������
	virtual BOOL DeleteAllItems();
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