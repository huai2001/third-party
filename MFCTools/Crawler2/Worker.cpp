#include "Stdafx.h"
#include "CrawlerDialog.h"
#include <MMSystem.h>

#ifdef _DEBUG
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
	#define new DEBUG_NEW
#endif

#define _READ_LINK_ 1

LPCTSTR gProductName = _T("抢单器 V1.0");
tagCrawlerConfig gCrawlerConfig = {0};

TCHAR szPathFile[MAX_PATH] = {0};
TCHAR szSoundFile[MAX_PATH] = {0};

BOOL _CC_CALL_ SaveConfigFile()
{
    BYTE nFileVersion = 0;
    BYTE cbBuffer[sizeof(tagCrawlerConfig)];

    nFileVersion = _encryptMapping(cbBuffer, (byte_t*)&gCrawlerConfig, sizeof(tagCrawlerConfig));

    FILE *fp = _tfopen(szPathFile, _T("wb"));
    if (fp == NULL) {
        return FALSE;
    }

    fwrite(&nFileVersion, sizeof(byte_t), 1, fp);
    fwrite(cbBuffer, sizeof(byte_t), sizeof(tagCrawlerConfig), fp);
    fclose(fp);

    return TRUE;
}

BOOL _CC_CALL_ ReadConfigFile()
{
    BYTE nFileVersion = 0;
    BYTE cbBuffer[sizeof(gCrawlerConfig)];
    size_t nReadSize = 0;

    FILE *fp = _tfopen(szPathFile, _T("rb"));
    if (fp == NULL) {
        return FALSE;
    }

    if (fread(&nFileVersion, sizeof(BYTE), 1, fp) != 1) {
        fclose(fp);
        return FALSE;
    }

    nReadSize = fread(&cbBuffer, sizeof(BYTE), sizeof(tagCrawlerConfig), fp);
    if (nReadSize < sizeof(tagCrawlerConfig)) {
        return FALSE;
    }

    fclose(fp);

    if (_decryptMapping((byte_t*)(&gCrawlerConfig), cbBuffer, sizeof(tagCrawlerConfig)) != nFileVersion) {
        _cc_logger_error(_T("Could't read config file: 0x%0.2x"), nFileVersion);
        return FALSE;
    }

    return TRUE;
}

int32_t _CC_CALL_ doStartingWorker(_cc_thread_t *t, pvoid_t args) {
	CCrawlerDialog *pWnd = (CCrawlerDialog*)args;
    HWND hWnd = pWnd->GetFromWindow();
    HWND hDesktop = ::GetDesktopWindow();

    CRect rc;

    if (gCrawlerConfig.nWidth <= 0 || gCrawlerConfig.nHeight <= 0) {
        MessageBox(0,_T("配置的窗口大小有问题，请重新配置！"), gProductName, MB_OK);
        pWnd->PostMessage(WM_MESSAGE_EVENT, EVENT_WORKER_CLOSEED, 0);
        return 0;
    }

    ::ShowWindow(hWnd, SW_SHOWNORMAL);
    ::SetWindowPos(hWnd,HWND_TOPMOST, 0, 0, gCrawlerConfig.nWidth, gCrawlerConfig.nHeight, 0);

    GetWindowRect(hWnd, &rc);
    BOOL isSoundPlay = pWnd->GetIsPlaySound();
    BOOL isNewOrder = FALSE;

    while(pWnd->IsStarting() && IsWindow(hWnd)) {
        HDC hdc = ::GetDC(hDesktop);
        tagPOINT point;
        COLORREF pixel[_cc_countof(gCrawlerConfig.mousePoint)];

        for (int i = 0; i < _cc_countof(gCrawlerConfig.mousePoint); i++) {
            point = gCrawlerConfig.mousePoint[i];
            pixel[i] = ::GetPixel(hdc, rc.left + point.x, rc.top + point.y);
        }
        
        //确认下单
        if (pixel[4] == gCrawlerConfig.colors[4] && 
            pixel[5] == gCrawlerConfig.colors[5] && 
            pixel[6] == gCrawlerConfig.colors[6]) {

            //点击银行卡
            point = gCrawlerConfig.mousePoint[5];
            MouseClicked(rc, point);
            Sleep(500);

            //选项卡偏移
            point.y += 35;
            MouseClicked(rc, point);
            Sleep(100);

            if (isSoundPlay) {
                PlaySound(szSoundFile, NULL, SND_FILENAME | SND_ASYNC);
            }

            //提交订单
            MouseClicked(rc, gCrawlerConfig.mousePoint[6]);

            isNewOrder = TRUE;
            Sleep(gCrawlerConfig.delayTime);
            continue;
        }

        if (isNewOrder == TRUE) {
            break;
        }
        
        if (pixel[0] == gCrawlerConfig.colors[0] && pixel[2] == gCrawlerConfig.colors[2]) {
            //出现订单
            MouseClicked(rc, gCrawlerConfig.mousePoint[2]);
            Sleep(100);
            continue;
        }

        //提交下单
        if (pixel[0] != gCrawlerConfig.colors[0] && pixel[3] == gCrawlerConfig.colors[3]) {
            MouseClicked(rc, gCrawlerConfig.mousePoint[2]);
            Sleep(100);
            continue;
        }

        //刷新
        if (pixel[0] == gCrawlerConfig.colors[0]) {
            MouseClicked(rc, gCrawlerConfig.mousePoint[1]);
        } else {
            tagPOINT point = gCrawlerConfig.mousePoint[0];
            point.y -= 20;
            MouseClicked(rc, point);
            Sleep(100);
        }

        Sleep(gCrawlerConfig.delayTime);
    }
    ::SetWindowPos(hWnd,HWND_NOTOPMOST, 0, 0, 0, 0,SWP_NOSIZE);

	pWnd->PostMessage(WM_MESSAGE_EVENT, EVENT_WORKER_CLOSEED, 0);
	return 1;
}

void _CC_CALL_ MouseClicked(tagRECT &rc, tagPOINT point) {
    tagPOINT oldMousePoint;
    ::GetCursorPos(&oldMousePoint);

    CPoint mousePoint(rc.left + point.x, rc.top + point.y);
    
    DWORD dx = mousePoint.x * 65536 / GetSystemMetrics(SM_CXSCREEN);
    DWORD dy = mousePoint.y * 65536 / GetSystemMetrics(SM_CYSCREEN);
    
    mouse_event(MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE,dx,dy,0,0);
    Sleep(1);
    mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
    Sleep(1);
    mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
    Sleep(1);
    ::SetCursorPos(oldMousePoint.x, oldMousePoint.y);
}