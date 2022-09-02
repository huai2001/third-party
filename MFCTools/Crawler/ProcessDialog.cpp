// ProcessDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Crawler.h"
#include "ProcessDialog.h"
#include "afxdialogex.h"
#include "Hook.h"

// CProcessDialog �Ի���

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


// CProcessDialog ��Ϣ�������
BOOL CProcessDialog::OnInitDialog()
{
	__super::OnInitDialog();
	
	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	// ִ�д˲���
	SetIcon(hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(hIcon, FALSE);			// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_ListCtrl.InitView();
	m_ModuleListCtrl.InitView();

	m_SkinScrollBar.InitScroolBar(&m_ListCtrl);
	m_SkinScrollBar.InitScroolBar(&m_ModuleListCtrl);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

typedef struct
{
	HWND hWnd; // ���ھ��
	DWORD dwProcessID; // ����ID
}tagEnumWindowsArgs;

///< ö�ٴ��ڻص�����
BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	tagEnumWindowsArgs *pArg = (tagEnumWindowsArgs *)lParam;
	DWORD dwProcessID = 0;
	// ͨ�����ھ��ȡ�ý���ID
	::GetWindowThreadProcessId(hWnd, &dwProcessID);
	if (dwProcessID == pArg->dwProcessID) {
		pArg->hWnd = hWnd;
		// �ҵ��˷���FALSE
		return FALSE;
	}
	// û�ҵ��������ң�����TRUE
	return TRUE;
}

//˫���б�
void CProcessDialog::OnNMDBLClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW *pNMListCtrl = (NM_LISTVIEW *)pNMHDR;
	//��¼ѡ�е���
	INT iItem = pNMListCtrl->iItem;
	//��¼ѡ�е���
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

//����б�
void CProcessDialog::OnNMLClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW *pNMListCtrl = (NM_LISTVIEW *)pNMHDR;
	//��¼ѡ�е���
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