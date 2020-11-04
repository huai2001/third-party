
// Crawler.cpp : 定义应用程序的类行为。
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


// CCrawlerApp 构造

CCrawlerApp::CCrawlerApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	ZeroMemory(&gCrawlerConfig, sizeof(gCrawlerConfig));
	ZeroMemory(&gCrawlerScreenshot, sizeof(gCrawlerScreenshot));
}


// 唯一的一个 CCrawlerApp 对象

CCrawlerApp theApp;


// CCrawlerApp 初始化

BOOL CCrawlerApp::InitInstance()
{
	CWinApp::InitInstance();

	_cc_install_dumper(NULL);
	_cc_create_socket();
	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	//环境配置
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
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}
	QuitHook();
	_cc_destroy_socket();
	_cc_uninstall_dumper();
	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

