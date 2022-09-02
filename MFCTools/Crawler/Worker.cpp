#include "Stdafx.h"
#include "CrawlerDialog.h"

#ifdef _DEBUG
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
	#define new DEBUG_NEW
#endif

#define _READ_LINK_ 1

LPCTSTR gProductName = _T("数据采集器 V1.0");
tagCrawlerConfig gCrawlerConfig = {0};
tagCrawlerScreenshot gCrawlerScreenshot = {0};

static DWORD dwBaseAddress = 0;
static DWORD dwRootAddress = 0;

static DWORD dwOffset = 0;//(sizeof(tagListPtr) - sizeof(tagLink));
static DWORD dwDataPtrSize = 0;
static tagDataPtr* DataPtrArray = NULL;
static DWORD dwDataPtrCount = 0;
static DWORD dwDataPtrOffset = 0;

#define _READ_LINK_		1
#define _READ_TREE_		2
#define _READ_ARRAY_	3

#define _READ_MODE_		_READ_TREE_

#if _READ_MODE_ == _READ_ARRAY_
//排序数组偏移地址
static DWORD dwRootOffset = sizeof(DWORD) * 4;
#elif _READ_MODE_ == _READ_LINK_
static DWORD dwRootOffset = -4;
#elif _READ_MODE_ == _READ_TREE_
//map 对象偏移地址
static DWORD dwRootOffset = sizeof(DWORD);
#endif

bool_t _CC_CALL_ ReadArrayCount (HANDLE hProcess) {
	DWORD i = 0, dwSize = 0;
	//基址
	DWORD dwOffsetValue = 0;
	DWORD dwAddress = dwBaseAddress;
	DWORD dwBaseAddressValue = dwBaseAddress;
	DWORD dwOffsetList[6] = {0x00173AD8, 0x08, 0x0408, 0x30, 0x24, 0x07A8};
	dwDataPtrOffset = 0;
	dwDataPtrCount = 0;

	for (i = 0; i < _cc_countof(dwOffsetList); i++) {
		dwAddress = dwBaseAddressValue + dwOffsetList[i];
		if (0 == ReadProcessMemory(hProcess, (LPVOID)dwAddress, &dwBaseAddressValue, sizeof(DWORD), &dwSize)) {
			return FALSE;
		}
	}

	//保存数值
	dwDataPtrCount = dwBaseAddressValue;

	//自选中没有数据
	if (dwDataPtrCount <= 0) {
		return TRUE;
	}
	//分配内存
	if (DataPtrArray == NULL || dwDataPtrCount > dwDataPtrSize) {
		if (DataPtrArray) {
			_cc_free(DataPtrArray);
		}
		dwDataPtrSize = dwDataPtrCount + 5;
		DataPtrArray = (tagDataPtr*)_cc_malloc(sizeof(tagDataPtr) * dwDataPtrSize);
		if (DataPtrArray == NULL) {
			return FALSE;
		}
	}

	//读取根节点地址
	if (0 == ReadProcessMemory(hProcess, (LPVOID)(dwAddress - dwRootOffset), &dwRootAddress, sizeof(DWORD), &dwSize)) {
		return FALSE;
	}
	return TRUE;
}

#if _READ_MODE_ == _READ_TREE_
bool_t _CC_CALL_ ReadTreeList(HANDLE hProcess, tagTreePtr *head) {
	DWORD dwSize;
	tagTreePtr ptr = {0};
	if (head->tree.left != dwRootAddress) {
		if (0 == ReadProcessMemory(hProcess, (LPVOID)(head->tree.left - dwOffset), &ptr, sizeof(tagTreePtr), &dwSize)) {
			return FALSE;
		}
		ptr.data.dwData[4] = head->tree.left;
		memcpy(DataPtrArray + dwDataPtrOffset++, &ptr.data, sizeof(tagDataPtr));
		if (dwDataPtrOffset > dwDataPtrCount) {
			return FALSE;
		}

		if(!ReadTreeList(hProcess, &ptr)) {
			return FALSE;
		}
	}

	if (head->tree.right != dwRootAddress) {
		if (0 == ReadProcessMemory(hProcess, (LPVOID)(head->tree.right - dwOffset), &ptr, sizeof(tagTreePtr), &dwSize)) {
			return FALSE;
		}
		ptr.data.dwData[4] = head->tree.right;
		memcpy(DataPtrArray + dwDataPtrOffset++, &ptr.data, sizeof(tagDataPtr));
		
		if (dwDataPtrOffset > dwDataPtrCount) {
			return FALSE;
		}

		if(!ReadTreeList(hProcess, &ptr)) {
			return FALSE;
		}
	}
	return TRUE;
}
#endif

int32_t _CC_CALL_ doStartingWorker(_cc_thread_t *t, pvoid_t args) {
	CCrawlerDialog *pWnd = (CCrawlerDialog*)args;
	tagProcessModule* pDataMgr = NULL;
	clock_t start, end;
	HANDLE hProcess = INVALID_HANDLE_VALUE;

#if _READ_MODE_ == _READ_TREE_
	tagTreePtr ptr = {0};
#endif

	EnableDebugPrivileges();

	pDataMgr = FindProcessModule(_T("DataMgr.dll"));//

	if (pDataMgr == NULL) {
		goto _WORKER_FAIL;
	}

	dwBaseAddress = pDataMgr->dwBaseAddr;
	
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pWnd->GetProcessID());
	if (hProcess == NULL){
		AfxMessageBox(_T("进程打开失败"));
		goto _WORKER_FAIL;
	}

	while (pWnd->IsStarting()) {
		DWORD i = 0;
		DWORD dwSize = 0;

		start = clock();
		//读取数量和分配置内存
		if (ReadArrayCount(hProcess) == FALSE) {
			break;
		}

		if (dwDataPtrCount > 0) {
			//读取数据地址
#if _READ_MODE_ == _READ_ARRAY_
			DWORD dwAddress = dwRootAddress;
			for (i = 0; i < dwDataPtrCount; i++) {
				if (0 == ReadProcessMemory(hProcess, (LPVOID)dwAddress, &DataPtrArray[i], sizeof(tagDataPtr), &dwSize)) {
					break;
				}
				dwDataPtrOffset++;
				dwAddress += sizeof(tagDataPtr);
			}

			for (i = 0; i < dwDataPtrOffset; i++) {
				tagDataPtr* p = (DataPtrArray + i);
					// 
					_CC_LOG_FMT(_CC_LOG_FLAGS_INFO_|_CC_LOG_FLAGS_UTF16_,_T("%p %p %p %p %p %p %p %p %p -- %ld, %ld, %ld, %ld"), 
					p->dwData[0], p->dwData[1], p->dwData[2], p->dwData[3],
					p->dwData[4],p->dwData[5],p->dwData[6],p->dwData[7],
					p->dwData[8],
					p->dwData[0], p->dwData[1], p->dwData[2], p->dwData[3]);
			}
#elif _READ_MODE_ == _READ_LINK_
			tagDoubleLink ptr;
			DWORD dwAddress = dwRootAddress;
			DWORD dwPtr[4];
			//读取链表根节点
			if (0 == ReadProcessMemory(hProcess, (LPVOID)dwAddress, &dwPtr, sizeof(dwPtr), &dwSize)) {
				break;
			}
			//取第一个节点
			dwAddress = dwPtr[2];
			while(1) {
				if (0 == ReadProcessMemory(hProcess, (LPVOID)dwAddress, &ptr, sizeof(tagDoubleLink), &dwSize)) {
					break;
				}
				//遍历完成
				if (ptr.prev == dwPtr[2]) {
					break;
				}

				_CC_DEBUG_LOG(_T("%p "), ptr.prev);
				dwAddress = ptr.prev;
				dwDataPtrOffset++;
			}
#elif _READ_MODE_ == _READ_TREE_
			DWORD parentPtr;
			//_CC_DEBUG_LOG(_T("%p "), dwRootAddress);
			//读取MAP地址
			//读取头指针
			if (0 == ReadProcessMemory(hProcess, (LPVOID)(dwRootAddress - dwOffset), &ptr, sizeof(tagTreePtr), &dwSize)) {
				break;
			}
			//memcpy(DataPtrArray + dwDataPtrOffset++, &ptr.data, sizeof(tagDataPtr));
			//_CC_DEBUG_LOG(_T("%p "), ptr.tree.parent);
			parentPtr = ptr.tree.parent;
			//读取第一个节点
			if (0 == ReadProcessMemory(hProcess, (LPVOID)(ptr.tree.parent - dwOffset), &ptr, sizeof(tagTreePtr), &dwSize)) {
				break;
			}
			ptr.data.dwData[4] = parentPtr;
			memcpy(DataPtrArray + dwDataPtrOffset++, &ptr.data, sizeof(tagDataPtr));
			//循环读取所有节点
			ReadTreeList(hProcess, &ptr);

			for (i = 0; i < dwDataPtrOffset; i++) {
				tagDataPtr* p = (DataPtrArray + i);
				// 
				_CC_LOG_FMT(_CC_LOG_FLAGS_INFO_|_CC_LOG_FLAGS_UTF16_,_T("%p %p %p %p %p %p %p %p %p -- %ld, %ld, %ld, %ld"), 
					p->dwData[0], p->dwData[1], p->dwData[2], p->dwData[3],
					p->dwData[4],p->dwData[5],p->dwData[6],p->dwData[7],
					p->dwData[8],
					p->dwData[0], p->dwData[1], p->dwData[2], p->dwData[3]);
			}
#endif
		}
		_cc_sleep(1000);
		end = clock();
		_CC_LOG_FMT(_CC_LOG_FLAGS_INFO_|_CC_LOG_FLAGS_UTF16_,_T("--%f---------%ld--%d----------------"),(double)(end - start) / CLOCKS_PER_SEC, dwDataPtrCount, dwDataPtrOffset);

	}

_WORKER_FAIL:
	if (DataPtrArray) {
		_cc_free(DataPtrArray);
		DataPtrArray = NULL;
	}
	dwDataPtrSize = 0;

	CloseHandle(hProcess);
	pWnd->PostMessage(WM_MESSAGE_EVENT, EVENT_WORKER_CLOSEED, 0);
	return 1;
}
