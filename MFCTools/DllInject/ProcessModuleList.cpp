#include "StdAfx.h"
#include "ProcessModuleList.h"

#ifdef _DEBUG
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
	#define new DEBUG_NEW
#endif

//标准构造函数
CProcessModuleList::CProcessModuleList(void)
{

}

//标准析构函数
CProcessModuleList::~CProcessModuleList(void)
{

}

BEGIN_MESSAGE_MAP(CProcessModuleList, CSkinListCtrl)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//ON_NOTIFY_REFLECT(NM_RCLICK, OnLvnItemRightClickList)
	//ON_NOTIFY_REFLECT(NM_CLICK, OnLvnItemLeftClickList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// CProcessModuleList 消息处理程序

//删除所有项
BOOL CProcessModuleList::DeleteAllItems()
{
	int c = GetItemCount();
	for (int i = 0; i < c; i++)
	{
		tagProcessModule* p = (tagProcessModule*)GetItemData(i);
		free(p);
	}

	return __super::DeleteAllItems();
}

//销毁
VOID CProcessModuleList::OnDestroy()
{
	DeleteAllItems();
	return __super::OnDestroy();
}
/*
//命令函数
BOOL CProcessModuleList::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	UINT nCommandID = LOWORD(wParam);

	switch(nCommandID)
	{
		break;
	}
	//自定义菜单事件
	return __super::OnCommand(wParam,lParam);
}

//右击列表
void CProcessModuleList::OnLvnItemLeftClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
}

//左击列表
void CProcessModuleList::OnLvnItemRightClickList(NMHDR *pNMHDR, LRESULT *pResult)
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
	*pResult = 0;
}
*/

void CProcessModuleList::Refresh(DWORD dwProcessId) {

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);
	MODULEENTRY32 me;
	ZeroMemory(&me, sizeof(MODULEENTRY32));
	me.dwSize = sizeof(MODULEENTRY32);

	SHFILEINFO sfi;
	bzero(&sfi, sizeof(SHFILEINFO));

	DeleteAllItems();

	int ret = Module32First(hSnap, &me);
	while (ret) {
		tagProcessModule *_left = (tagProcessModule*)malloc(sizeof(tagProcessModule));
		_tcsncpy(_left->szModule, me.szExePath, _cc_countof(_left->szModule));
		_left->szModule[_cc_countof(_left->szModule) - 1] = 0;
		_left->dwBaseAddr = (DWORD_PTR)me.modBaseAddr;
		_left->dwBaseSize = me.modBaseSize;
		_left->hIcon = NULL;
		/**/

		if (SHGetFileInfo(_left->szModule, FILE_ATTRIBUTE_DIRECTORY, &sfi, sizeof(SHFILEINFO), SHGFI_ICON) != 0) {
			_left->hIcon = sfi.hIcon;
		}
		
		InsertDataItem(_left);

		ret = Module32Next(hSnap, &me);

	}
	CloseHandle(hSnap);
}
//初始化
void CProcessModuleList::InitView()
{
	InsertColumn(GUI_PROCESS_MODULE_LIST_ICON_ID, GUI_PROCESS_MODULE_LIST_ICON,
		LVCFMT_LEFT,
		GUI_PROCESS_MODULE_LIST_ICON_SIZE
		);

	InsertColumn(GUI_PROCESS_MODULE_LIST_NAME_ID, GUI_PROCESS_MODULE_LIST_NAME,
		LVCFMT_LEFT,
		GUI_PROCESS_MODULE_LIST_NAME_SIZE
		);

	InsertColumn(GUI_PROCESS_MODULE_LIST_ADDRESS_ID, GUI_PROCESS_MODULE_LIST_ADDRESS,
		LVCFMT_LEFT,
		GUI_PROCESS_MODULE_LIST_ADDRESS_SIZE
		);

	InsertColumn(GUI_PROCESS_MODULE_LIST_BASESIZE_ID, GUI_PROCESS_MODULE_LIST_BASESIZE,
		LVCFMT_LEFT,
		GUI_PROCESS_MODULE_LIST_BASESIZE_SIZE
		);

	//设置行高
	SetItemHeight(26);

	m_bRectify = FALSE;

	m_SkinScrollBar.InitScroolBar(this);
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序
//  这将由框架自动完成。

void CProcessModuleList::OnSize(UINT nType, int cx, int cy) 
{
	__super::OnSize(nType, cx, cy);

	if(GetSafeHwnd() && m_bRectify == FALSE)
	{
		m_bRectify = TRUE;
	}
}

//排列数据

INT CProcessModuleList::SortItemData(LPARAM lParam1, LPARAM lParam2, WORD wColumnIndex, BOOL bAscendSort)
{
	//对比数据
	BOOL bSort = FALSE;
	int iCompRes = 1;

	//定义变量
	tagProcessModule *p1 = (tagProcessModule *)lParam1;
	tagProcessModule *p2 = (tagProcessModule *)lParam2;

	if(!p1 || !p2)
		return 1;

	switch(wColumnIndex)
	{
	case GUI_PROCESS_MODULE_LIST_ICON_ID:
	case GUI_PROCESS_MODULE_LIST_NAME_ID:
		{
			iCompRes = _tcsicmp(p1->szModule, p2->szModule);
			break;
		}
	case GUI_PROCESS_MODULE_LIST_ADDRESS_ID:
		{
			iCompRes = p1->dwBaseAddr > p2->dwBaseAddr ? SORT_AFTER : SORT_FRONT;
			break;
		}
	case GUI_PROCESS_MODULE_LIST_BASESIZE_ID:
		{
			iCompRes = p1->dwBaseSize > p2->dwBaseSize ? SORT_AFTER : SORT_FRONT;
			break;
		}
	}
	return (bAscendSort ? iCompRes: -iCompRes);
}

//获取位置
INT CProcessModuleList::GetInsertIndex(VOID * pItemData)
{
	//变量定义
	INT nItemCount = GetItemCount();

	return nItemCount;
}

//描述字符
VOID CProcessModuleList::ConstructString(VOID * pItemData, WORD wColumnIndex, LPTSTR pszString, WORD wMaxCount)
{
	//定义变量
	tagProcessModule *p = (tagProcessModule *)pItemData;

	if (p == NULL) {
		return ;
	}

	switch(wColumnIndex)
	{
	case GUI_PROCESS_MODULE_LIST_NAME_ID:
		{
			_tcsncpy(pszString, p->szModule, wMaxCount);
			pszString[wMaxCount - 1] = 0;
			break;
		}
	case GUI_PROCESS_MODULE_LIST_ADDRESS_ID:
		{
			_sntprintf(pszString, wMaxCount, _T("%p"), p->dwBaseAddr);
			break;
		}
	case GUI_PROCESS_MODULE_LIST_BASESIZE_ID:
		{
			_sntprintf(pszString, wMaxCount, _T("%p"), p->dwBaseSize);
			break;
		}
	}
	return;
}

//绘画数据
VOID CProcessModuleList::DrawCustomItem(CDC * pDC, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect&rcSubItem, INT nColumnIndex)
{
	//定义变量
	tagProcessModule *p = (tagProcessModule *)lpDrawItemStruct->itemData;

	if (p == NULL) {
		return ;
	}

	if (nColumnIndex == GUI_PROCESS_MODULE_LIST_ICON_ID && p->hIcon) {
		DrawIconEx(pDC->GetSafeHdc(), rcSubItem.left + 3, rcSubItem.top + (m_uItemHeight - 16) / 2, p->hIcon, 16, 16, 0, NULL, DI_NORMAL | DI_COMPAT);
		return;
	}
	//默认调用
	__super::DrawCustomItem(pDC,lpDrawItemStruct,rcSubItem,nColumnIndex);

	return;
}