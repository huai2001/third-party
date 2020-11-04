
// Crawler.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "Crawler.h"
#include "CrawlerDialog.h"
#include "Hook.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCrawlerApp

BEGIN_MESSAGE_MAP(CCrawlerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCrawlerApp ����

CCrawlerApp::CCrawlerApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	ZeroMemory(&gCrawlerConfig, sizeof(gCrawlerConfig));
	ZeroMemory(&gCrawlerScreenshot, sizeof(gCrawlerScreenshot));
}


// Ψһ��һ�� CCrawlerApp ����

CCrawlerApp theApp;


// CCrawlerApp ��ʼ��

BOOL CCrawlerApp::InitInstance()
{
	CWinApp::InitInstance();

	_cc_install_dumper(NULL);
	_cc_create_socket();
	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	//��������
	AfxOleInit();
	AfxInitRichEdit2();
	EnableDebugPrivileges();

	tagSkinRenderInfo SkinRenderInfo;

	SkinRenderInfo.wColorH = DEF_COLOR_H;
	SkinRenderInfo.dRectifyS = DEF_COLOR_S;
	SkinRenderInfo.dRectifyB = DEF_COLOR_B;

	m_SkinResourceManager.SetSkinResource(SkinRenderInfo);

	CCrawlerDialog dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}
	QuitHook();
	_cc_destroy_socket();
	_cc_uninstall_dumper();
	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

