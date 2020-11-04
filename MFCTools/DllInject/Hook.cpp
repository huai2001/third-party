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
// 函    数: EnableDebugPrivileges
// 描    述: 提升进程权限
// 参    数: 无
// 返 回 值:
//    int -- 成功返回 TRUE
//           失败返回 FALSE
// 备    注:
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
// 函    数: InjectDLL
// 描    述: 远程线程注入DLL
// 参    数: 
//			dwProcessID 进程ID
//			szPath 需要注入的DLL
// 返 回 值:
//    int -- 成功返回 TRUE
//           失败返回 FALSE
// 备    注:
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
// 函    数: UnInjectDLL
// 描    述: 远程线程注入DLL
// 参    数: 
//			dwProcessID 进程ID
//			szModuleName 需要卸载的DLL名称
// 返 回 值:
//    int -- 成功返回 TRUE
//           失败返回 FALSE
// 备    注:
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