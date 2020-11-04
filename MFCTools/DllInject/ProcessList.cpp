#include "StdAfx.h"
#include "ProcessList.h"
#include "InjectDialog.h"

#ifdef _DEBUG
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
	#define new DEBUG_NEW
#endif

//��׼���캯��
CProcessList::CProcessList(void)
{
	m_hKernel32 = GetModuleHandle(_T("kernel32"));
	if (m_hKernel32) {
		m_funcIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(m_hKernel32 ,"IsWow64Process");
	} else {
		m_funcIsWow64Process = NULL;
	}
}

//��׼��������
CProcessList::~CProcessList(void)
{
	/*if (m_hKernel32) {
		CloseHandle(m_hKernel32);
	}*/
}

BEGIN_MESSAGE_MAP(CProcessList, CSkinListCtrl)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//ON_NOTIFY_REFLECT(NM_RCLICK, OnLvnItemRightClickList)
	//ON_NOTIFY_REFLECT(NM_CLICK, OnLvnItemLeftClickList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// CProcessList ��Ϣ�������

//ɾ��������
BOOL CProcessList::DeleteAllItems()
{
	int c = GetItemCount();
	for (int i = 0; i < c; i++)
	{
		tagProcessData* p = (tagProcessData*)GetItemData(i);
		free(p->szModule);
		free(p->szFullPath);
		free(p);
	}

	return __super::DeleteAllItems();
}

//����
VOID CProcessList::OnDestroy()
{
	DeleteAllItems();
	return __super::OnDestroy();
}
/*
tagProcessData *pProcessData = NULL;

//�����
BOOL CProcessList::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID = LOWORD(wParam);
	CInjectDialog inject;
	
	switch(nCommandID)
	{
	case IDM_INJECT_DLL:{
		inject.SetProcessData(pProcessData);
		inject.DoModal();
		}
		return TRUE;
	case IDM_UNINJECT_DLL: {
		inject.SetProcessData(pProcessData);
		inject.DoModal();
		}
		return TRUE;
	case IDM_REFRESH_PROCESS: {
		Refresh();
		}
		return TRUE;
	}
	
	//�Զ���˵��¼�
	return __super::OnCommand(wParam,lParam);
}

//�һ��б�
void CProcessList::OnLvnItemLeftClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
}

//����б�
void CProcessList::OnLvnItemRightClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if(pNMLV->iItem == -1) {
		*pResult = 0;
		return ;
	}

	DWORD_PTR dwData = (DWORD_PTR)GetItemData(pNMLV->iItem);

	//��������
	if(dwData == -1) {
		return;
	}
	

	pProcessData = (tagProcessData*)dwData;
	//����˵�
	CSkinMenu Menu;
	Menu.CreateMenu();

	Menu.AppendMenu(IDM_ABOUTBOX,TEXT("����..."));
	//����˵�
	Menu.AppendSeparator();
	Menu.AppendMenu(IDM_INJECT_DLL,TEXT("ע��DLL"));
	Menu.AppendMenu(IDM_UNINJECT_DLL,TEXT("ж��DLL"));
	Menu.AppendSeparator();
	Menu.AppendMenu(IDM_REFRESH_PROCESS,TEXT("ˢ��"));

	//��ʾ�˵�
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	Menu.TrackPopupMenu(MousePoint.x,MousePoint.y,this);
	*pResult = 0;
}
*/
BOOL DosPathToNtPath(LPTSTR pszDosPath, LPTSTR pszNtPath)
{
	TCHAR szDriveStr[500];
	TCHAR szDrive[3];
	TCHAR szDevName[100];
	INT nDevNameLen;
	INT i;

	//������
	if(!pszDosPath || !pszNtPath )
		return FALSE;
	//��ȡ���ش����ַ���
	if (GetLogicalDriveStrings(sizeof(szDriveStr), szDriveStr)) {
		for(i = 0; szDriveStr[i]; i += 4) {
			if(!lstrcmpi(&(szDriveStr[i]), _T("A:\\")) || !lstrcmpi(&(szDriveStr[i]), _T("B:\\")))
				continue;

			szDrive[0] = szDriveStr[i];
			szDrive[1] = szDriveStr[i + 1];
			szDrive[2] = '\0';

			if(!QueryDosDevice(szDrive, szDevName, 100))//��ѯ Dos �豸��
				return FALSE;

			nDevNameLen = lstrlen(szDevName);
			if(_tcsnicmp(pszDosPath, szDevName, nDevNameLen) == 0) {
				lstrcpy(pszNtPath, szDrive);//����������
				lstrcat(pszNtPath, pszDosPath + nDevNameLen);//����·��

				return TRUE;
			}           
		}
	}

	pszNtPath[0] = 0;

	//lstrcpy(pszNtPath, pszDosPath);

	return FALSE;
}

void CProcessList::Refresh() {
	HANDLE hSnap;
	PROCESSENTRY32 pe32 = {0};

	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hSnap == INVALID_HANDLE_VALUE) {
		//_CC_ERROR_LOG(_T("[-]CreateToolhelp32Snapshot with error %s\n"), _cc_last_error(_cc_last_errno()));
		return ;
	}

	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hSnap, &pe32)) {
		CloseHandle(hSnap);
		return;
	}

	DeleteAllItems();

	do {
		TCHAR szExePath[MAX_PATH] = {0};
		TCHAR szImagePath[MAX_PATH] = {0};
		HICON hIcon = NULL;
		BOOL b32Bit = TRUE;
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pe32.th32ProcessID);
		if (!hProcess) {
			Process32Next(hSnap, &pe32);
			continue;
		}
		//��ȡ����·�������һ�ȡ����ͼ��
		if(GetProcessImageFileName(hProcess, szImagePath, MAX_PATH)) {
			
			if (DosPathToNtPath(szImagePath, szExePath)) {
				SHFILEINFO sfi;
				bzero(&sfi, sizeof(SHFILEINFO));
				if (SHGetFileInfo(szExePath, FILE_ATTRIBUTE_DIRECTORY, &sfi, sizeof(SHFILEINFO), SHGFI_ICON) != 0) {
					hIcon = sfi.hIcon;
				}
			}
		}

		if (m_funcIsWow64Process) {
			if (!m_funcIsWow64Process(hProcess,&b32Bit)) {
				b32Bit = TRUE;
			}
		}
		CloseHandle(hProcess);

		tagProcessData *p = (tagProcessData*)malloc(sizeof(tagProcessData));
		if (p) {
			p->szModule = _tcsdup(pe32.szExeFile);
			p->szFullPath = _tcsdup(szExePath);
			p->dwProcessID = pe32.th32ProcessID;
			p->dwThreadCount = pe32.cntThreads;
			p->hIcon = hIcon;
			p->b32Bit = b32Bit;
			InsertDataItem(p);
		}
	} while(Process32Next(hSnap, &pe32));

	CloseHandle(hSnap);
}
//��ʼ��
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

	InsertColumn(GUI_PROCESS_LIST_BIT_ID, GUI_PROCESS_LIST_BIT,
		LVCFMT_LEFT,
		GUI_PROCESS_LIST_BIT_SIZE
		);
	InsertColumn(GUI_PROCESS_LIST_FULLPATH_ID, GUI_PROCESS_LIST_FULLPATH,
		LVCFMT_LEFT,
		GUI_PROCESS_LIST_FULLPATH_SIZE
		);
	//�����и�
	SetItemHeight(26);

	//ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	m_bRectify = FALSE;

	m_SkinScrollBar.InitScroolBar(this);
	Refresh();
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CProcessList::OnSize(UINT nType, int cx, int cy) 
{
	__super::OnSize(nType, cx, cy);

	if(GetSafeHwnd() && m_bRectify == FALSE)
	{
		m_bRectify = TRUE;
	}
}

//��������

INT CProcessList::SortItemData(LPARAM lParam1, LPARAM lParam2, WORD wColumnIndex, BOOL bAscendSort)
{
	//�Ա�����
	BOOL bSort = FALSE;
	int iCompRes = 1;

	//�������
	tagProcessData *p1 = (tagProcessData *)lParam1;
	tagProcessData *p2 = (tagProcessData *)lParam2;

	if(!p1 || !p2)
		return 1;

	switch(wColumnIndex)
	{
	case GUI_PROCESS_LIST_ICON_ID:
	case GUI_PROCESS_LIST_NAME_ID:
		{
			iCompRes = _tcsicmp(p1->szModule, p2->szModule);
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
	case GUI_PROCESS_LIST_BIT_ID:
		{
			iCompRes = p1->b32Bit > p2->b32Bit ? SORT_AFTER : SORT_FRONT;
			break;
		}
	case GUI_PROCESS_LIST_FULLPATH_ID:
		{
			iCompRes = _tcsicmp(p1->szFullPath, p2->szFullPath);
			break;
		}
	}
	return (bAscendSort ? iCompRes: -iCompRes);
}

//��ȡλ��
INT CProcessList::GetInsertIndex(VOID * pItemData)
{
	//��������
	INT nItemCount = GetItemCount();

	return nItemCount;
}

//�����ַ�
VOID CProcessList::ConstructString(VOID * pItemData, WORD wColumnIndex, LPTSTR pszString, WORD wMaxCount)
{
	//�������
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
			_sntprintf(pszString, wMaxCount, _T("%ld"), p->dwProcessID);
			break;
		}
	case GUI_PROCESS_LIST_THREADS_ID:
		{
			_sntprintf(pszString, wMaxCount, _T("%ld"), p->dwThreadCount);
			break;
		}
	case GUI_PROCESS_LIST_BIT_ID:
		{
			_sntprintf(pszString, wMaxCount, _T("%s"), (p->b32Bit == TRUE ? _T("32 λ") : _T("64 λ")));
			break;
		}
	case GUI_PROCESS_LIST_FULLPATH_ID:
		{
			_tcsncpy(pszString, p->szFullPath, wMaxCount);
			pszString[wMaxCount - 1] = 0;
			break;
		}
	}
	return;
}

//�滭����
VOID CProcessList::DrawCustomItem(CDC * pDC, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect&rcSubItem, INT nColumnIndex)
{
	//�������
	tagProcessData *p = (tagProcessData *)lpDrawItemStruct->itemData;

	if (p == NULL) {
		return ;
	}

	if (nColumnIndex == GUI_PROCESS_LIST_ICON_ID && p->hIcon) {
		DrawIconEx(pDC->GetSafeHdc(), rcSubItem.left + 3, rcSubItem.top + ((rcSubItem.Height() - 16 ) / 2), p->hIcon, 16, 16, 0, NULL, DI_NORMAL | DI_COMPAT);
		return;
	}

	//Ĭ�ϵ���
	__super::DrawCustomItem(pDC,lpDrawItemStruct,rcSubItem,nColumnIndex);

	return;
}