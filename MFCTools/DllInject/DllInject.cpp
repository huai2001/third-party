
// DllInject.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "DllInject.h"
#include "DllInjectDailog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDllInjectApp

BEGIN_MESSAGE_MAP(CDllInjectApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CDllInjectApp ����

CDllInjectApp::CDllInjectApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDllInjectApp ����

CDllInjectApp theApp;


// CDllInjectApp ��ʼ��

BOOL CDllInjectApp::InitInstance()
{
	CWinApp::InitInstance();

	//CoInitialize(NULL);
	//��������
	//AfxOleInit();
	//AfxInitRichEdit2();

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
	tagSkinRenderInfo SkinRenderInfo;

	SkinRenderInfo.wColorH = DEF_COLOR_H;
	SkinRenderInfo.dRectifyS = DEF_COLOR_S;
	SkinRenderInfo.dRectifyB = DEF_COLOR_B;

	m_SkinResourceManager.SetSkinResource(SkinRenderInfo);

	CDllInjectDailog dlg;
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

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

