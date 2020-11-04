#include "stdafx.h"
#include "Hook.h"

////////////////////////////////////////////////////////////////////
// copy from msdn
typedef struct _LSA_UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} LSA_UNICODE_STRING, *PLSA_UNICODE_STRING, UNICODE_STRING, *PUNICODE_STRING;

typedef enum _MEMORY_INFORMATION_CLASS {
	MemoryBasicInformation,
	MemoryWorkingSetList,
	MemorySectionName
} MEMORY_INFORMATION_CLASS;

typedef NTSTATUS (WINAPI *ZwQueryVirtualMemoryFunc) (
	_In_      HANDLE                   ProcessHandle,
	_In_opt_  PVOID                    BaseAddress,
	_In_      MEMORY_INFORMATION_CLASS MemoryInformationClass,
	_Out_     PVOID                    MemoryInformation,
	_In_      SIZE_T                   MemoryInformationLength,
	_Out_opt_ PSIZE_T                  ReturnLength
	);
////////////////////////////////////////////////////////////////////
typedef struct {
	UNICODE_STRING SectionFileName;
	WCHAR NameBuffer[MAX_PATH * 2 + 2];
} MEMORY_SECTION_NAME, *PMEMORY_SECTION_NAME;

static _cc_rb_tree_t gProcessModuleMap = {NULL};
static ZwQueryVirtualMemoryFunc gZwQueryVirtualMemoryPtr = NULL;
///////////////////////////////////////////////////
// ��    ��: CCQueryVirtualMemory
// ��    ��: ZwQueryVirtualMemory ��{��}{��}����
// ��    ��:
//    [url]https://msdn.microsoft.com/en-us/library/windows/hardware/dn957455[/url](v=vs.85).aspx
// �� �� ֵ:
//    int -- �ɹ����� TRUE
//           ʧ�ܷ��� FALSE
// ����ʱ��: 2017/06/06 23:39:04
// ��    ע:
//         
///////////////////////////////////////////////////
static bool_t WINAPI ZwQueryVirtualMemory(HANDLE ProcessHandle, PVOID BaseAddress, MEMORY_INFORMATION_CLASS MemoryInformationClass, PVOID MemoryInformation, ULONG MemoryInformationLength)
{
	if(gZwQueryVirtualMemoryPtr == NULL) {
		gZwQueryVirtualMemoryPtr = (ZwQueryVirtualMemoryFunc)GetProcAddress(GetModuleHandleA("ntdll.dll"), "ZwQueryVirtualMemory");
		if(gZwQueryVirtualMemoryPtr == NULL) {
			return FALSE;
		}
	}

	if(gZwQueryVirtualMemoryPtr(ProcessHandle, 
								BaseAddress, 
								MemoryInformationClass, 
								MemoryInformation, 
								MemoryInformationLength, NULL) > 0 ) {
			return TRUE;
	}

	return FALSE;
}

static void _CC_CALL_ _PushMap(HICON hIcon, const tchar_t* szModule, DWORD dwBaseAddress, DWORD dwBaseSize) {
	tagProcessModule *_left = NULL;
	int32_t result = 0;
	_cc_rb_node_t **node = &(gProcessModuleMap.rb_node), *parent = NULL;

	while (*node) {
		_left = _cc_upcast(*node, tagProcessModule, node);
		result = _tcscmp(_left->szModule, szModule);

		parent = *node;

		if (result < 0) {
			node = &((*node)->right);
		} else if (result > 0) {
			node = &((*node)->left);
		} else {
			result = 1;
			break;
		}
	}
	if (result == 1) {
		_left->dwBaseAddr = dwBaseAddress;
		_left->dwBaseSize = dwBaseSize;
		_left->hIcon = hIcon;
	} else {
		tagProcessModule *_left = (tagProcessModule*)_cc_malloc(sizeof(tagProcessModule));
		_tcsncpy(_left->szModule, szModule, _cc_countof(_left->szModule));
		_left->szModule[_cc_countof(_left->szModule) - 1] = 0;
		_left->dwBaseAddr = dwBaseAddress;
		_left->dwBaseSize = dwBaseSize;
		_left->hIcon = hIcon;
		_cc_rb_insert(&gProcessModuleMap, &_left->node, parent, node);
	}
}

static int32_t _CC_CALL_ _FindMap(_cc_rb_node_t* left, pvoid_t args) {
	tagProcessModule *_left = _cc_upcast(left, tagProcessModule, node);

	return _tcscmp((const tchar_t*)args, _left->szModule);
}

static void _CC_CALL_ _FreeMap(_cc_rb_node_t *node) {
	tagProcessModule *item = _cc_upcast(node, tagProcessModule, node);
	_cc_free(item);
}
///////////////////////////////////////////////////
// ��    ��: EnumProcessModule
// ��    ��: ��ȡָ������ָ��ģ���ַ
// ��    ��:
//    DWORD dwProcessID -- ����PID
// �� �� ֵ: ��
// ��    ע:
//
///////////////////////////////////////////////////
void _CC_CALL_ EnumProcessModule(DWORD dwProcessID)
{
	_cc_rb_destroy(&gProcessModuleMap, _FreeMap);
#if 1
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessID);
	MODULEENTRY32 me;
	ZeroMemory(&me, sizeof(MODULEENTRY32));
	me.dwSize = sizeof(MODULEENTRY32);

	int ret = Module32First(hSnap, &me);
	while (ret) {
		_PushMap(NULL, me.szModule, (DWORD)me.modBaseAddr, me.modBaseSize);
		ret = Module32Next(hSnap, &me);
	}
	CloseHandle(hSnap);
#else
	SIZE_T siReturnLength;
	DWORD dwStepAddress = 0x00000000;
	MEMORY_BASIC_INFORMATION mbi;
	BYTE szBuffer[MAX_PATH * 2 + 4] = {0};
	PUNICODE_STRING uniSectionName;
	NTSTATUS nStatus = 0;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, dwProcessID);

	ZwQueryVirtualMemoryFunc fnZwQueryVirtualMemory = NULL;

	fnZwQueryVirtualMemory = (ZwQueryVirtualMemoryFunc)::GetProcAddress(GetModuleHandleA("ntdll.dll"),"ZwQueryVirtualMemory" );
	if (fnZwQueryVirtualMemory) {
		for (dwStepAddress = 0; dwStepAddress <= 0x7FFFFFFF; dwStepAddress += 0x10000) {
			nStatus = fnZwQueryVirtualMemory(hProcess,(PVOID)dwStepAddress, MemoryBasicInformation, &mbi, sizeof(MEMORY_BASIC_INFORMATION), &siReturnLength);
			if (nStatus < 0 || mbi.Type != SEC_IMAGE) {
				continue;
			}
			
			nStatus = fnZwQueryVirtualMemory(hProcess,(PVOID)dwStepAddress, MemorySectionName, szBuffer, sizeof(szBuffer), &siReturnLength);
			if (nStatus < 0) {
				continue;
			}

			uniSectionName = (PUNICODE_STRING)szBuffer;

			SHFILEINFO stFileInfo;
			bzero(&stFileInfo, sizeof(SHFILEINFO));

			SHGetFileInfo(uniSectionName->Buffer, 0, &stFileInfo, sizeof(SHFILEINFO), SHGFI_ICON);

			_PushMap(stFileInfo.hIcon, _tcsrchr(uniSectionName->Buffer,_T('\\')) + 1, (DWORD)mbi.BaseAddress, mbi.RegionSize);
		}
	}
#endif
	return ;
}

tagProcessModule* _CC_CALL_ FindProcessModule(const tchar_t *szModuleName) {
	_cc_rb_node_t *node = _cc_rb_get(&gProcessModuleMap, (pvoid_t)szModuleName, _FindMap);
	if (node) {
		return _cc_upcast(node, tagProcessModule, node);
	}
	return NULL;
}

_cc_rb_node_t* _CC_CALL_ FirstProcessModule() {
	return _cc_rb_first(&gProcessModuleMap);
}

_cc_rb_node_t* _CC_CALL_ LastProcessModule() {
	return _cc_rb_last(&gProcessModuleMap);
}

void _CC_CALL_ QuitHook() {
	_cc_rb_destroy(&gProcessModuleMap, _FreeMap);
}

///////////////////////////////////////////////////
// ��    ��: EnumProcess
// ��    ��: ����ָ������
// ��    ��:
//    bool_t func(LPPROCESSENTRY32) -- �ص�����,����TRUE �ж�ѭ��
// �� �� ֵ: ��
// ��    ע:
//         
///////////////////////////////////////////////////
void _CC_CALL_ EnumProcess(bool_t (_CC_CALL_ *func)(LPPROCESSENTRY32, pvoid_t), pvoid_t args)
{
	HANDLE hSnap;
	PROCESSENTRY32 pe32 = {0};

	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hSnap == INVALID_HANDLE_VALUE) {
		_CC_ERROR_LOG(_T("[-]CreateToolhelp32Snapshot with error %s\n"), _cc_last_error(_cc_last_errno()));
		return ;
	}

	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hSnap, &pe32)) {
		do {
			if (func(&pe32, args)) {
				break;
			}
		} while(Process32Next(hSnap, &pe32));
	}
	CloseHandle(hSnap);
}

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
bool_t _CC_CALL_ EnableDebugPrivileges()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	LUID Luid;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
		_CC_ERROR_LOG(_T("[-] OpenProcessToken error with %s"),_cc_last_error(_cc_last_errno()));
		return FALSE;
	}

	if(!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &Luid )) {
		_CC_ERROR_LOG(_T("[-] LookupPrivilegeValue error with %s"),_cc_last_error(_cc_last_errno()));
		CloseHandle(hToken);
		return FALSE;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = Luid;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if(!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL)) {
		_CC_ERROR_LOG(_T("[-] AdjustTokenPrivileges error with %s"),_cc_last_error(_cc_last_errno()));
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
bool_t _CC_CALL_ InjectDLL(DWORD dwProcessID, tchar_t *szPath) {
	bool_t r = FALSE;
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

	WriteProcessMemory(hProcess, pAddress, szPath, nPathLength, NULL);

	HANDLE hModule = (HANDLE)_cc_load_object(_T("KERNEL32.DLL"));
	if (hModule) {
		LPTHREAD_START_ROUTINE lpStartAddress = (LPTHREAD_START_ROUTINE)_cc_load_function(hModule, "LoadLibraryA");
		HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, lpStartAddress, pAddress, 0, NULL);
		if (hThread) {
			WaitForSingleObject(hThread, 2000);

			CloseHandle(hThread);
			r = TRUE;
		}
		_cc_unload_object(hModule);
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
bool_t _CC_CALL_ unInjectDLL(DWORD dwProcessID, tchar_t *szModuleName) {
	bool_t r = FALSE;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
	if (!hProcess) {
		return FALSE;
	}
	
	LPVOID lpAddress = NULL;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessID);
	MODULEENTRY32 me;
	ZeroMemory(&me, sizeof(MODULEENTRY32));
	me.dwSize = sizeof(MODULEENTRY32);

	int ret = Module32First(hSnap, &me);
	while (ret) {
		if (_tcsicmp(szModuleName, me.szModule) == 0) {
			lpAddress = (LPVOID)me.modBaseAddr;
			break;
		}
		ret = Module32Next(hSnap, &me);
	}
	CloseHandle(hSnap);

	if (lpAddress == NULL) {
		CloseHandle(hProcess);
		return FALSE;
	}

	HANDLE hModule = (HANDLE)_cc_load_object(_T("KERNEL32.DLL"));
	if (hModule) {
		LPTHREAD_START_ROUTINE lpStartAddress = (LPTHREAD_START_ROUTINE)_cc_load_function(hModule, "FreeLibrary");
		HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, lpStartAddress, lpAddress, 0, NULL);
		if (hThread) {
			WaitForSingleObject(hThread, 2000);

			CloseHandle(hThread);
			r = TRUE;
		}
		_cc_unload_object(hModule);
	}

	CloseHandle(hProcess);
	return r;
}
/*
DWORD_PTR dwModuleBaseAddr = 0;
DWORD dwProcessID;
HANDLE hProcess = INVALID_HANDLE_VALUE;
int Open(DWORD dwProcessID, TCHAR *pModule) {
	//��ȡ����ģ��ƫ��ֵ
	HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessID);
	MODULEENTRY32 me;
	int ret = Module32First(h, &me);
	while (ret) {
		if (_tcsicmp(me.szModule, pModule) == 0) {
			dwModuleBaseAddr = (DWORD_PTR)me.modBaseAddr;
			break;
		}
		ret = Module32Next(h, &me);
	}
	CloseHandle(h);
	//�򿪽���
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
	if (hProcess == NULL){
		AfxMessageBox("���̴�ʧ��\n");
		return -1;
	}
	return 0;
}


double ReadBalance(DWORD dwOffsetList[], DWORD dwCount) {
	double r = 0;
	DWORD dwNum = 0, dwSize = 0;
	//��ַ
	DWORD dwBaseAddress = 0;
	DWORD dwOffsetValue = 0;
	DWORD dwBaseAddressValue = dwModuleBaseAddr;

	if (dwModuleBaseAddr == 0) {
		AfxMessageBox("��ַ��Ч");
		return 0;
	}

	if (hProcess == INVALID_HANDLE_VALUE) {
		AfxMessageBox("������Ч");
	}

	for (dwNum = 0; dwNum < dwCount; dwNum++) {
		dwBaseAddress = dwBaseAddressValue + dwOffsetList[dwNum];
		if (0 == ReadProcessMemory(hProcess, (LPVOID)dwBaseAddress, &dwBaseAddressValue, sizeof(DWORD), &dwSize)) {
			AfxMessageBox("��ַ̬��ȡʧ��\n");
			return -1;
		}
	}
	dwBaseAddress = dwBaseAddressValue + dwOffsetList[dwNum];
	if (0 == ReadProcessMemory(hProcess, (LPVOID)dwBaseAddress, &r, sizeof(double), &dwSize)) {
		AfxMessageBox("��ַ̬��ȡʧ��\n");
		return -1;
	}
	return r;
}
*/