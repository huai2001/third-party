// NBService.cpp : �������̨Ӧ�ó������ڵ㡣
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

static int nServiceStatus = 0;//����״̬


static void WINAPI ServiceHandler(DWORD fdwControl) {  
    switch(fdwControl) {
    case SERVICE_CONTROL_STOP:
    case SERVICE_CONTROL_SHUTDOWN:
        ServiceStatus.dwWin32ExitCode = 0;
        ServiceStatus.dwCurrentState  = SERVICE_STOPPED;
        ServiceStatus.dwCheckPoint    = 0;
        ServiceStatus.dwWaitHint      = 0;

        //ֹͣ����
        nServiceStatus = 0;
        _cc_logger_debug(_T("ֹͣ����"));

        break;
    case SERVICE_CONTROL_PAUSE://��ͣ����
        ServiceStatus.dwWaitHint = 10000;
        ServiceStatus.dwCurrentState = SERVICE_PAUSE_PENDING;
        _cc_logger_debug(_T("������ͣ������"));

        if (!SetServiceStatus(hServiceStatusHandle,  &ServiceStatus)) {
            _cc_logger_error(_T("SetServiceStatus fail:%s"), _cc_last_error(_cc_last_errno()));
            return;
        }

        ServiceStatus.dwCurrentState = SERVICE_PAUSED;

        _cc_logger_debug(_T("��ͣ����"));

        //��ͣ����
        if (nServiceStatus == 1) {
            nServiceStatus = 2;
        }

        break;
    case SERVICE_CONTROL_CONTINUE:
        ServiceStatus.dwCurrentState = SERVICE_CONTINUE_PENDING;
        _cc_logger_debug(_T("���ڼ���������"));
        if (!SetServiceStatus(hServiceStatusHandle,  &ServiceStatus)) {
            _cc_logger_error(_T("SetServiceStatus fail:%s"), _cc_last_error(_cc_last_errno()));
            return;
        }

        _cc_logger_debug(_T("��������"));
        ServiceStatus.dwCurrentState = SERVICE_RUNNING;

        //��������
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
    _cc_logger_debug(_T("���÷���"));
    nServiceStatus = 1;
}

void WINAPI ServiceMain(int argc, char** argv) {
    ServiceStatus.dwServiceType        = SERVICE_WIN32;
    ServiceStatus.dwCurrentState       = SERVICE_START_PENDING;
    //�ڱ�����ֻ����ϵͳ�ػ���ֹͣ�������ֿ�������
    ServiceStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PAUSE_CONTINUE;
    ServiceStatus.dwWin32ExitCode      = 0;
    ServiceStatus.dwServiceSpecificExitCode = 0;
    ServiceStatus.dwCheckPoint         = 0;
    ServiceStatus.dwWaitHint           = 0;
    //ע�������ƴ���������������ָ��ControlHandlerfunction ��ָ�룩������״̬������ SCM ��������
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

    // ��������Ŀ��Ʒ��ɻ��߳�
    StartServiceCtrlDispatcher(ServiceTable);
#endif
	return 0;
}

