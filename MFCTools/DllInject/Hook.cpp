#include "stdafx.h"
#include "Hook.h"
#pragma comment(lib, "psapi.lib")

#ifndef _DEBUG
	#ifndef WIN64
		#pragma comment (lib,"../Release/SkinControls.lib")
	#else
		#pragma comment (lib,"../x64/Release/SkinControls.lib")
	#endif
#else
	#ifndef WIN64
		#pragma comment (lib,"../Debug/SkinControlsD.lib")
	#else
		#pragma comment (lib,"../x64/Debug/SkinControlsD.lib")
	#endif
#endif
///////////////////////////////////////////////////
// ��    ��: EnableDebugPrivileges
// ��    ��: ��������Ȩ��
// ��    ��: ��
// �� �� ֵ:
//    int -- �ɹ����� TRUE
//           ʧ�ܷ��� FALSE
// ��    ע:
//         
///////////////////////////////////////////////////
BOOL EnableDebugPrivileges()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	LUID Luid;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
		//_CC_ERROR_LOG(_T("[-] OpenProcessToken error with %s"),_cc_last_error(_cc_last_errno()));
		return FALSE;
	}

	if(!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &Luid )) {
		//_CC_ERROR_LOG(_T("[-] LookupPrivilegeValue error with %s"),_cc_last_error(_cc_last_errno()));
		CloseHandle(hToken);
		return FALSE;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = Luid;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if(!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL)) {
		//_CC_ERROR_LOG(_T("[-] AdjustTokenPrivileges error with %s"),_cc_last_error(_cc_last_errno()));
		CloseHandle(hToken);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////
// ��    ��: InjectDLL
// ��    ��: Զ���߳�ע��DLL
// ��    ��: 
//			dwProcessID ����ID
//			szPath ��Ҫע���DLL
// �� �� ֵ:
//    int -- �ɹ����� TRUE
//           ʧ�ܷ��� FALSE
// ��    ע:
//         
///////////////////////////////////////////////////
BOOL  InjectDLL(DWORD dwProcessID, TCHAR *szPath) {
	BOOL r = FALSE;

	EnableDebugPrivileges();

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
	if (!hProcess) {
		return FALSE;
	}

	size_t nPathLength = _tcslen(szPath) + 1;

	LPVOID pAddress = VirtualAllocEx(hProcess, NULL, nPathLength, MEM_COMMIT, PAGE_READWRITE);
	if (!pAddress) {
		CloseHandle(hProcess);
		return FALSE;
	}

	WriteProcessMemory(hProcess, pAddress, szPath, nPathLength * sizeof(TCHAR), NULL);

	HMODULE hModule = (HMODULE)GetModuleHandle(_T("KERNEL32"));
	if (hModule) {
#ifdef UNICODE
		LPTHREAD_START_ROUTINE lpStartAddress = (LPTHREAD_START_ROUTINE)GetProcAddress(hModule, "LoadLibraryW");
#else
		LPTHREAD_START_ROUTINE lpStartAddress = (LPTHREAD_START_ROUTINE)GetProcAddress(hModule, "LoadLibraryA");
#endif
		HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, lpStartAddress, pAddress, 0, NULL);
		if (hThread) {
			WaitForSingleObject(hThread, 2000);

			CloseHandle(hThread);
			r = TRUE;
		}
	}

	CloseHandle(hProcess);
	return r;
}

///////////////////////////////////////////////////
// ��    ��: UnInjectDLL
// ��    ��: Զ���߳�ע��DLL
// ��    ��: 
//			dwProcessID ����ID
//			szModuleName ��Ҫж�ص�DLL����
// �� �� ֵ:
//    int -- �ɹ����� TRUE
//           ʧ�ܷ��� FALSE
// ��    ע:
//         
///////////////////////////////////////////////////
BOOL _CC_CALL_ unInjectDLL(DWORD dwProcessID, DWORD_PTR dwBaseAddress) {
	BOOL r = FALSE;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
	if (!hProcess) {
		return FALSE;
	}

	HMODULE hModule = (HMODULE)GetModuleHandle(_T("KERNEL32"));
	if (hModule) {
		LPTHREAD_START_ROUTINE lpStartAddress = (LPTHREAD_START_ROUTINE)GetProcAddress(hModule, "FreeLibrary");
		HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, lpStartAddress, (LPVOID)dwBaseAddress, 0, NULL);
		if (hThread) {
			WaitForSingleObject(hThread, 2000);

			CloseHandle(hThread);
			r = TRUE;
		}
	}

	CloseHandle(hProcess);
	return r;
}