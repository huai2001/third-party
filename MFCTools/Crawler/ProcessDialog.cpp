// ProcessDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "Crawler.h"
#include "ProcessDialog.h"
#include "afxdialogex.h"
#include "Hook.h"

// CProcessDialog 对话框

CProcessDialog::CProcessDialog(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CProcessDialog::IDD, pParent)
{

}

CProcessDialog::~CProcessDialog()
{

}

void CProcessDialog::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROCESS_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_PROCESS_MODULE_LIST, m_ModuleListCtrl);
}


BEGIN_MESSAGE_MAP(CProcessDialog, CSkinDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_PROCESS_LIST, OnNMDBLClickList)
	ON_NOTIFY(NM_CLICK, IDC_PROCESS_LIST, OnNMLClickList)
END_MESSAGE_MAP()


// CProcessDialog 消息处理程序
BOOL CProcessDialog::OnInitDialog()
{
	__super::OnInitDialog();
	
	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	// 执行此操作
	SetIcon(hIcon, TRUE);			// 设置大图标
	SetIcon(hIcon, FALSE);			// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_ListCtrl.InitView();
	m_ModuleListCtrl.InitView();

	m_SkinScrollBar.InitScroolBar(&m_ListCtrl);
	m_SkinScrollBar.InitScroolBar(&m_ModuleListCtrl);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

typedef struct
{
	HWND hWnd; // 窗口句柄
	DWORD dwProcessID; // 进程ID
}tagEnumWindowsArgs;

///< 枚举窗口回调函数
BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	tagEnumWindowsArgs *pArg = (tagEnumWindowsArgs *)lParam;
	DWORD dwProcessID = 0;
	// 通过窗口句柄取得进程ID
	::GetWindowThreadProcessId(hWnd, &dwProcessID);
	if (dwProcessID == pArg->dwProcessID) {
		pArg->hWnd = hWnd;
		// 找到了返回FALSE
		return FALSE;
	}
	// 没找到，继续找，返回TRUE
	return TRUE;
}

//双击列表
void CProcessDialog::OnNMDBLClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW *pNMListCtrl = (NM_LISTVIEW *)pNMHDR;
	//记录选中的行
	INT iItem = pNMListCtrl->iItem;
	//记录选中的列
	INT iSubItem = pNMListCtrl->iSubItem;
	if (pNMListCtrl->iItem != -1) {
		tagProcessData* p = (tagProcessData*)m_ListCtrl.GetItemData(iItem);
		tagEnumWindowsArgs arg;
		arg.hWnd = NULL;
		arg.dwProcessID = p->dwProcessID;
		EnumWindows(EnumWindowsProc, (LPARAM)&arg);

		if (arg.hWnd) {
			GetParent()->SendMessage(WM_MESSAGE_EVENT, EVENT_PROCESS_ATTACH, (LPARAM)arg.hWnd);
		}
		__super::OnCancel();
	}
}

//点击列表
void CProcessDialog::OnNMLClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW *pNMListCtrl = (NM_LISTVIEW *)pNMHDR;
	//记录选中的行
	INT iItem = pNMListCtrl->iItem;
	if (pNMListCtrl->iItem != -1) {
		tagProcessData *p = (tagProcessData*)m_ListCtrl.GetItemData(iItem);
		EnumProcessModule(p->dwProcessID);
		m_ModuleListCtrl.DeleteAllItems();
		_cc_rbtree_iterator_t *v;
		for (v = FirstProcessModule(); v != NULL; v = _cc_rb_next(v)) {
			m_ModuleListCtrl.InsertDataItem(_cc_upcast(v, tagProcessModule, node));
		}
	}
}