// NBService.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Windows.h"
#include <libcc.h>

#define SERVICE_NAME "NBService"

static SERVICE_STATUS ServiceStatus;
static SERVICE_STATUS_HANDLE hServiceStatusHandle;
static void WINAPI ServiceMain(int argc, char** argv);
static void WINAPI ServiceHandler(DWORD fdwControl);

static TCHAR szServiceName[128];
static SC_HANDLE schSCManager;
static SC_HANDLE schService;

static int nServiceStatus = 0;//服务状态


static void WINAPI ServiceHandler(DWORD fdwControl) {  
    switch(fdwControl) {
    case SERVICE_CONTROL_STOP:
    case SERVICE_CONTROL_SHUTDOWN:
        ServiceStatus.dwWin32ExitCode = 0;
        ServiceStatus.dwCurrentState  = SERVICE_STOPPED;
        ServiceStatus.dwCheckPoint    = 0;
        ServiceStatus.dwWaitHint      = 0;

        //停止服务
        nServiceStatus = 0;
        _cc_logger_debug(_T("停止服务"));

        break;
    case SERVICE_CONTROL_PAUSE://暂停服务
        ServiceStatus.dwWaitHint = 10000;
        ServiceStatus.dwCurrentState = SERVICE_PAUSE_PENDING;
        _cc_logger_debug(_T("正在暂停服务中"));

        if (!SetServiceStatus(hServiceStatusHandle,  &ServiceStatus)) {
            _cc_logger_error(_T("SetServiceStatus fail:%s"), _cc_last_error(_cc_last_errno()));
            return;
        }

        ServiceStatus.dwCurrentState = SERVICE_PAUSED;

        _cc_logger_debug(_T("暂停服务"));

        //暂停服务
        if (nServiceStatus == 1) {
            nServiceStatus = 2;
        }

        break;
    case SERVICE_CONTROL_CONTINUE:
        ServiceStatus.dwCurrentState = SERVICE_CONTINUE_PENDING;
        _cc_logger_debug(_T("正在继续服务中"));
        if (!SetServiceStatus(hServiceStatusHandle,  &ServiceStatus)) {
            _cc_logger_error(_T("SetServiceStatus fail:%s"), _cc_last_error(_cc_last_errno()));
            return;
        }

        _cc_logger_debug(_T("继续服务"));
        ServiceStatus.dwCurrentState = SERVICE_RUNNING;

        //继续服务
        if (nServiceStatus == 2) {
            nServiceStatus = 1;
        }
        break;
    default:
        return;
    };

    if (!SetServiceStatus(hServiceStatusHandle,  &ServiceStatus)) {
        _cc_logger_error(_T("SetServiceStatus fail:%s"), _cc_last_error(_cc_last_errno()));
    }
}

static void Init(void) {
    _cc_logger_debug(_T("启用服务"));
    nServiceStatus = 1;
}

void WINAPI ServiceMain(int argc, char** argv) {
    ServiceStatus.dwServiceType        = SERVICE_WIN32;
    ServiceStatus.dwCurrentState       = SERVICE_START_PENDING;
    //在本例中只接受系统关机和停止服务两种控制命令
    ServiceStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PAUSE_CONTINUE;
    ServiceStatus.dwWin32ExitCode      = 0;
    ServiceStatus.dwServiceSpecificExitCode = 0;
    ServiceStatus.dwCheckPoint         = 0;
    ServiceStatus.dwWaitHint           = 0;
    //注册服务控制处理器（服务名和指向ControlHandlerfunction 的指针），控制状态器处理 SCM 控制请求
    hServiceStatusHandle = RegisterServiceCtrlHandler(_T(SERVICE_NAME), ServiceHandler);
    
    if (hServiceStatusHandle == 0) {
        _cc_logger_error(_T("RegisterServiceCtrlHandler fail:%s"), _cc_last_error(_cc_last_errno()));
    }

    // Initialization complete - report running status   
    ServiceStatus.dwCurrentState       = SERVICE_RUNNING;
    ServiceStatus.dwCheckPoint         = 0;
    ServiceStatus.dwWaitHint           = 9000;

    if (!SetServiceStatus(hServiceStatusHandle, &ServiceStatus)) {
        _cc_logger_error(_T("RegisterServiceCtrlHandler fail:%s"), _cc_last_error(_cc_last_errno()));
        return ;
    }

    Init();
}

int _tmain(int argc, _TCHAR* argv[]) {
#ifdef _CC_DEBUG_
    Init();
   _cc_uninstall_memory_tracked();
#else
    SERVICE_TABLE_ENTRY ServiceTable[2];  

    ServiceTable[0].lpServiceName = _T(SERVICE_NAME);
    ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;

    ServiceTable[1].lpServiceName = NULL;
    ServiceTable[1].lpServiceProc = NULL;

    // 启动服务的控制分派机线程
    StartServiceCtrlDispatcher(ServiceTable);
#endif
	return 0;
}

