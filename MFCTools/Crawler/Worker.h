#ifndef WORKER_HEAD_FILE
#define WORKER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Hook.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

byte_t _CC_CALL_ _decryptMapping(byte_t *source, byte_t *encrypt, int32_t encrypt_size);
byte_t _CC_CALL_ _encryptMapping(byte_t *encrypt, byte_t *source, int32_t source_size);

#define WM_MESSAGE_EVENT	(WM_USER + 2000)

#define EVENT_DRAG_STARTED			10
#define EVENT_DRAG_ENDED			11
#define EVENT_SETTING_CLOSE			12
#define EVENT_WORKER_INITIALIZED	13
#define EVENT_WORKER_CLOSEED		14
#define EVENT_OPEN_PROCESS_ATTACH	15
#define EVENT_PROCESS_ATTACH		16

#define MAX_CRAWLER_DATA_BOX 3

#pragma pack(1)
typedef struct tagProducts {
	INT nId;
	TCHAR szName[128];
	TCHAR szField[MAX_CRAWLER_DATA_BOX][32];
}tagProducts;

//
typedef struct tagDataPtr {
	DWORD dwData[9];
}tagDataPtr;

typedef struct tagDoubleLink {
	DWORD prev;
	DWORD next;
}tagDoubleLink;

typedef struct tagTreeLink{
	DWORD left;
	DWORD parent;
	DWORD right;
	DWORD color;
}tagTreeLink;

typedef struct tagTreePtr {
	tagTreeLink tree;
	tagDataPtr data;
}tagTreePtr;



typedef struct tagCrawlerScreenshot {
	BYTE cbSuccess;
	LONG nBoxWidth[MAX_CRAWLER_DATA_BOX];
	tagRECT rcBox;
	TCHAR szTitle[128];
	TCHAR szClassName[128];
}tagCrawlerScreenshot;

typedef struct tagCrawlerConfig {
	BYTE cbSuccess;
	BYTE cbPlatform;
	TCHAR szHost[32];
	USHORT wPort;
	TCHAR szDBName[32];
	TCHAR szUser[32];
	TCHAR szPassword[32];
	_cc_sql_t *pConnectSQL;
	tagProducts *pProducts;
	INT nProductCount;
}tagCrawlerConfig;

#pragma pack()

extern LPCTSTR gProductName;

int32_t _CC_CALL_ doStartingWorker(_cc_thread_t *t, pvoid_t args);
//////////////////////////////////////////////////////////////////////////
/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif