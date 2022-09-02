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

#pragma pack(1)

typedef struct tagCrawlerConfig {
	BYTE cbSuccess;
	BYTE cbPlatform;
    
	TCHAR szTitle[128];
	TCHAR szClassName[128];

    tagPOINT mousePoint[7];
    DWORD colors[7];
    DWORD delayTime;
    INT nWidth;
    INT nHeight;
}tagCrawlerConfig;

#pragma pack()

extern tagCrawlerConfig gCrawlerConfig;
extern LPCTSTR gProductName;
extern TCHAR szPathFile[MAX_PATH];
extern TCHAR szSoundFile[MAX_PATH];

int32_t _CC_CALL_ doStartingWorker(_cc_thread_t *t, pvoid_t args);
BOOL _CC_CALL_ ReadConfigFile();
BOOL _CC_CALL_ SaveConfigFile();
void _CC_CALL_ MouseClicked(tagRECT &rc, tagPOINT point);
//////////////////////////////////////////////////////////////////////////
/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif