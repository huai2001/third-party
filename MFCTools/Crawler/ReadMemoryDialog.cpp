// ReadMemoryDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Crawler.h"
#include "ReadMemoryDialog.h"
#include "afxdialogex.h"
#include "Hook.h"


// CReadMemoryDialog �Ի���

IMPLEMENT_DYNAMIC(CReadMemoryDialog, CDialog)

CReadMemoryDialog::CReadMemoryDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CReadMemoryDialog::IDD, pParent)
{
	m_hProcess = NULL;
}

CReadMemoryDialog::~CReadMemoryDialog()
{
}

void CReadMemoryDialog::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MEMORY_DATA, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CReadMemoryDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_READ, &CReadMemoryDialog::OnBnClickedButtonRead)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MEMORY_DATA, OnNMDBLClickList)
END_MESSAGE_MAP()


// CReadMemoryDialog ��Ϣ�������

BOOL CReadMemoryDialog::OnInitDialog()
{
	__super::OnInitDialog();
	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(hIcon, FALSE);		// ����Сͼ��


	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	// Set the ListCtrl style
	LONG lStyle = m_ListCtrl.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
	lStyle |=  LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	m_ListCtrl.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0,(LPARAM)lStyle);

	// Add ListCtrl titles
	m_ListCtrl.InsertColumn(0,_T("��ַ"),NULL,150, -1);
	m_ListCtrl.InsertColumn(1,_T("ָ��"),NULL,120,-1);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
//˫���б�
void CReadMemoryDialog::OnNMDBLClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	TCHAR szBuffer[1024];
	NM_LISTVIEW *pNMListCtrl = (NM_LISTVIEW *)pNMHDR;
	//��¼ѡ�е���
	INT iItem = pNMListCtrl->iItem;
	//��¼ѡ�е���
	INT iSubItem = pNMListCtrl->iSubItem;
	if (pNMListCtrl->iItem != -1) {
		m_ListCtrl.GetItemText(iItem, iSubItem, szBuffer, _cc_countof(szBuffer));
		_cc_set_clipboard_text(szBuffer);
	}
}


void CReadMemoryDialog::Open(DWORD dwProcessID, CWnd *pParenWnd) {

	if (GetSafeHwnd() == NULL) {
		Create(CReadMemoryDialog::IDD, pParenWnd);
	}

	ShowWindow(SW_SHOW);
	
	m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
	if (m_hProcess == NULL){
		AfxMessageBox(_T("���̴�ʧ��"));
		return ;
	}

	return ;
}

void CReadMemoryDialog::ReadMemory(DWORD  dwBaseAddress, INT nCount) {

	INT i = 0;
	TCHAR szBuf[128];
	if (nCount > 0) {
		dwBaseAddress -= sizeof(DWORD) * nCount;
		nCount += nCount;
	}
	nCount += 1;

	DWORD dwSize = 0;
	//��ַ
	DWORD dwBaseAddressValue = 0;
	m_ListCtrl.DeleteAllItems();

	for (i = 0; i < nCount; i++) {
		if (0 == ReadProcessMemory(m_hProcess, (LPVOID)dwBaseAddress, &dwBaseAddressValue, sizeof(DWORD), &dwSize)) {
			break;
		}
		_sntprintf(szBuf, _cc_countof(szBuf), _T("%p"), dwBaseAddress);
		INT nItem = m_ListCtrl.InsertItem(m_ListCtrl.GetItemCount(),szBuf);
		_sntprintf(szBuf, _cc_countof(szBuf), _T("%p"), dwBaseAddressValue);
		m_ListCtrl.SetItem(nItem,1,1,szBuf,NULL,0,0,0);
		dwBaseAddress += sizeof(DWORD);
	}
}

void CReadMemoryDialog::OnBnClickedButtonRead()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR szBuf[128];
	GetDlgItemText(IDC_EDIT_MEMORY_ADDRESS, szBuf, _cc_countof(szBuf));

	ReadMemory(_cc_hex8(szBuf), GetDlgItemInt(IDC_EDIT_MEMORY_LENGTH));
}