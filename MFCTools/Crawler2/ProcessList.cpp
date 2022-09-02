#include "StdAfx.h"
#include "ProcessList.h"

#ifdef _DEBUG
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
	#define new DEBUG_NEW
#endif

bool_t _CC_CALL_ EnumWindowsProc(LPPROCESSENTRY32 pe, pvoid_t args) {
	CProcessList *pListCtrl = (CProcessList*)args;
	TCHAR szExePath[_CC_MAX_PATH_] = {0};
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe->th32ProcessID);
	if (!hProcess) {
		return FALSE;
	}

	::GetModuleFileNameEx((HMODULE)hProcess,NULL,szExePath,sizeof(szExePath));
	CloseHandle(hProcess);

	if (szExePath[0] == 0) {
		return FALSE;
	}

	tagProcessData *p = (tagProcessData*)malloc(sizeof(tagProcessData));
	if (p) {
		p->szModule = _tcsdup(pe->szExeFile);
		p->dwProcessID = pe->th32ProcessID;
		p->dwThreadCount = pe->cntThreads;
		p->hIcon = NULL;
		SHFILEINFO sfi;
		bzero(&sfi, sizeof(SHFILEINFO));

		if(SHGetFileInfo(szExePath, FILE_ATTRIBUTE_DIRECTORY, &sfi, sizeof(SHFILEINFO), SHGFI_ICON) != 0) {
			p->hIcon = sfi.hIcon;
		}

		pListCtrl->InsertDataItem(p);
	}
	return FALSE;
}

//标准构造函数
CProcessList::CProcessList(void)
{

}

//标准析构函数
CProcessList::~CProcessList(void)
{
	/*
*/
}

BEGIN_MESSAGE_MAP(CProcessList, CSkinListCtrl)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//ON_NOTIFY_REFLECT(NM_RCLICK, OnLvnItemRightClickList)
	//ON_NOTIFY_REFLECT(NM_CLICK, OnLvnItemLeftClickList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// CProcessList 消息处理程序

//删除所有项
BOOL CProcessList::DeleteAllItems()
{
	int c = GetItemCount();
	for (int i = 0; i < c; i++)
	{
		tagProcessData* p = (tagProcessData*)GetItemData(i);
		free(p->szModule);
		free(p);
	}

	return __super::DeleteAllItems();
}

//销毁
VOID CProcessList::OnDestroy()
{
	DeleteAllItems();
	return __super::OnDestroy();
}

//命令函数
BOOL CProcessList::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	UINT nCommandID = LOWORD(wParam);
	/*
	switch(nCommandID)
	{
		break;
	}
	*/
	//自定义菜单事件
	return __super::OnCommand(wParam,lParam);
}
/*
//右击列表
void CProcessList::OnLvnItemLeftClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
}

//左击列表
void CProcessList::OnLvnItemRightClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if(pNMLV->iItem == -1) {
		*pResult = 0;
		return ;
	}

	DWORD_PTR dwData = (DWORD_PTR)GetItemData(pNMLV->iItem);

	//查找数据
	if(dwData == -1) {
		return;
	}

	//构造菜单
	CMenu InfoMenu;
	InfoMenu.CreatePopupMenu();

	InfoMenu.AppendMenu(MF_POPUP,IDM_ENABLE_PROXY, _T("启用代理"));
	InfoMenu.AppendMenu(MF_POPUP,IDM_DISABLED_PROXY, _T("禁用代理"));

	CPoint	pPoint;
	::GetCursorPos(&pPoint);     
	InfoMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pPoint.x, pPoint.y, this);
	*pResult = 0;
}
*/
//初始化
void CProcessList::InitView()
{
	InsertColumn(GUI_PROCESS_LIST_ICON_ID, GUI_PROCESS_LIST_ICON,
		LVCFMT_LEFT,
		GUI_PROCESS_LIST_ICON_SIZE
		);

	InsertColumn(GUI_PROCESS_LIST_NAME_ID, GUI_PROCESS_LIST_NAME,
		LVCFMT_LEFT,
		GUI_PROCESS_LIST_NAME_SIZE
		);

	InsertColumn(GUI_PROCESS_LIST_PID_ID, GUI_PROCESS_LIST_PID,
		LVCFMT_LEFT,
		GUI_PROCESS_LIST_PID_SIZE
		);

	InsertColumn(GUI_PROCESS_LIST_THREADS_ID, GUI_PROCESS_LIST_THREADS,
		LVCFMT_LEFT,
		GUI_PROCESS_LIST_THREADS_SIZE
		);

	//设置行高
	SetItemHeight(22);

	EnumProcess(EnumWindowsProc, (pvoid_t)this);

	m_bRectify = FALSE;
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序
//  这将由框架自动完成。

void CProcessList::OnSize(UINT nType, int cx, int cy) 
{
	__super::OnSize(nType, cx, cy);

	if(GetSafeHwnd() && m_bRectify == FALSE)
	{
		m_bRectify = TRUE;
	}
}

//排列数据

INT CProcessList::SortItemData(LPARAM lParam1, LPARAM lParam2, WORD wColumnIndex, bool_t bAscendSort)
{
	//对比数据
	BOOL bSort = FALSE;
	int iCompRes = 1;

	//定义变量
	tagProcessData *p1 = (tagProcessData *)lParam1;
	tagProcessData *p2 = (tagProcessData *)lParam2;

	if(!p1 || !p2)
		return 1;

	switch(wColumnIndex)
	{
	case GUI_PROCESS_LIST_NAME_ID:
		{
			iCompRes = _tcscmp(p1->szModule, p2->szModule) >= 0 ? SORT_AFTER : SORT_FRONT;
			break;
		}
	case GUI_PROCESS_LIST_PID_ID:
		{
			iCompRes = p1->dwProcessID > p2->dwProcessID ? SORT_AFTER : SORT_FRONT;
			break;
		}
	case GUI_PROCESS_LIST_THREADS_ID:
		{
			iCompRes = p1->dwThreadCount > p2->dwThreadCount ? SORT_AFTER : SORT_FRONT;
			break;
		}
	}
	return (bAscendSort == TRUE) ? iCompRes: - iCompRes;
}

//获取位置
INT CProcessList::GetInsertIndex(VOID * pItemData)
{
	//变量定义
	INT nItemCount = GetItemCount();

	return nItemCount;
}

//描述字符
VOID CProcessList::ConstructString(VOID * pItemData, WORD wColumnIndex, LPTSTR pszString, WORD wMaxCount)
{
	//定义变量
	tagProcessData *p = (tagProcessData *)pItemData;

	if (p == NULL) {
		return ;
	}

	switch(wColumnIndex)
	{
	case GUI_PROCESS_LIST_NAME_ID:
		{
			_tcsncpy(pszString, p->szModule, wMaxCount);
			pszString[wMaxCount - 1] = 0;
			break;
		}
	case GUI_PROCESS_LIST_PID_ID:
		{
			_sntprintf(pszString, wMaxCount, _T("%p"), p->dwProcessID);
			break;
		}
	case GUI_PROCESS_LIST_THREADS_ID:
		{
			_sntprintf(pszString, wMaxCount, _T("%ld"), p->dwThreadCount);
			break;
		}
	}
	return;
}

//绘画数据
VOID CProcessList::DrawCustomItem(CDC * pDC, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect&rcSubItem, INT nColumnIndex)
{
	//定义变量
	tagProcessData *p = (tagProcessData *)lpDrawItemStruct->itemData;

	if (p == NULL) {
		return ;
	}

	if (nColumnIndex == GUI_PROCESS_LIST_ICON_ID && p->hIcon) {
		DrawIconEx(pDC->GetSafeHdc(), rcSubItem.left + 3, rcSubItem.top + (m_uItemHeight - 20) / 2, p->hIcon, 16, 16, 0, NULL, DI_NORMAL | DI_COMPAT);
		return;
	}

	//默认调用
	__super::DrawCustomItem(pDC,lpDrawItemStruct,rcSubItem,nColumnIndex);

	return;
}