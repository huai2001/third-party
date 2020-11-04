// ProcessList.h : 头文件
//

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#define GUI_PROCESS_LIST_ICON_ID			0
#define GUI_PROCESS_LIST_ICON				_T("")
#define GUI_PROCESS_LIST_ICON_SIZE			22

#define GUI_PROCESS_LIST_NAME_ID			1
#define GUI_PROCESS_LIST_NAME				_T("进程名")
#define GUI_PROCESS_LIST_NAME_SIZE			300

#define GUI_PROCESS_LIST_PID_ID				2
#define GUI_PROCESS_LIST_PID				_T("进程ID")
#define GUI_PROCESS_LIST_PID_SIZE			70

#define GUI_PROCESS_LIST_THREADS_ID			3
#define GUI_PROCESS_LIST_THREADS			_T("线程数")
#define GUI_PROCESS_LIST_THREADS_SIZE		70

typedef struct tagProcessData {
	TCHAR* szModule;
	DWORD dwProcessID;
	DWORD dwThreadCount;
	HICON hIcon;
}tagProcessData;


class CProcessList : public CSkinListCtrl
{
	//控件变量
protected:
	BOOL							m_bRectify;						//调整标志

	//函数定义
public:
	//构造函数
	CProcessList();
	//析构函数
	virtual ~CProcessList();

	//重载函数
protected:
	//命令函数
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//删除所有项
	virtual BOOL DeleteAllItems();
	//数据控制
protected:
	//排列数据
	virtual INT SortItemData(LPARAM lParam1, LPARAM lParam2, word_t wColumnIndex, bool_t bAscendSort);
	//描述字符
	virtual VOID ConstructString(VOID * pItemData, WORD wColumnIndex, LPTSTR pszString, WORD wMaxCount);
	//获取位置
	virtual INT GetInsertIndex(VOID * pItemData);
	//绘画控制
protected:
	//绘画数据
	virtual VOID DrawCustomItem(CDC * pDC, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect&rcSubItem, INT nColumnIndex);
	//功能函数
public:
	//初始化
	void InitView();
// 实现
protected:
	/*//左击列表
	afx_msg void OnLvnItemRightClickList(NMHDR *pNMHDR, LRESULT *pResult);
	//右击列表
	afx_msg void OnLvnItemLeftClickList(NMHDR *pNMHDR, LRESULT *pResult);*/
	//大小消息
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//销毁
	afx_msg VOID OnDestroy();

	DECLARE_MESSAGE_MAP()
};