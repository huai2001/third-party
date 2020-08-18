#ifndef WORKER_HEAD_FILE
#define WORKER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "image.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

void _CC_CALL_ _cc_image_to_gray(_cc_image_t *src);
void _CC_CALL_ _cc_convert_bnary_bitmap(_cc_image_t* dst);
byte_t _CC_CALL_ _decryptMapping(byte_t *source, byte_t *encrypt, int32_t encrypt_size);
byte_t _CC_CALL_ _encryptMapping(byte_t *encrypt, byte_t *source, int32_t source_size);

#define WM_MESSAGE_EVENT	(WM_USER + 2000)

#define EVENT_DRAG_STARTED			10
#define EVENT_DRAG_ENDED			11
#define EVENT_SETTING_CLOSE			12
#define EVENT_WORKER_INITIALIZED	13
#define EVENT_WORKER_CLOSEED		14

#define MAX_CRAWLER_DATA_BOX 3

#pragma pack(1)
typedef struct tagProducts {
	INT nId;
	TCHAR szName[128];
	TCHAR szField[MAX_CRAWLER_DATA_BOX][32];
}tagProducts;

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

extern _cc_sql_driver_t gSQLDriver;
extern tagCrawlerScreenshot gCrawlerScreenshot;
extern tagCrawlerConfig gCrawlerConfig;
extern LPCTSTR gProductName;

_cc_image_t* CuttingImage(_cc_image_t *src, int left, int top, int width, int height);
//////////////////////////////////////////////////////////////////////////
bool_t _CC_CALL_ doConnectSQL();
void _CC_CALL_ doDisconnectSQL();
//1 数据库未连接
//2 查询数据库失败“products表”
//3 数据平台未配置
//4 内存分配失败
int32_t _CC_CALL_ doWorkerInitialization(_cc_thread_t *t, pvoid_t args);
int32_t _CC_CALL_ doStartingWorker(_cc_thread_t *t, pvoid_t args);
//////////////////////////////////////////////////////////////////////////
/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif