#include "Stdafx.h"
#include "image.h"
#include "CrawlerDialog.h"

#ifdef _DEBUG
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
	#define new DEBUG_NEW
#endif

LPCTSTR gProductName = _T("数据采集器 V1.0");
tagCrawlerConfig gCrawlerConfig = {0};
tagCrawlerScreenshot gCrawlerScreenshot = {0};
_cc_sql_driver_t gSQLDriver;

#define GRAY_LEVELS          256
//计算生成二值图的阈值
static int _calculate_binary_threshold(_cc_image_t* img) {
	int threshold = 0;
	//获取位图的宽
	int width = img->width;
	//获取位图的高
	int height = img->height;
	double num_tag[GRAY_LEVELS];
	double g_tmp = 0;
	double g_max = 0;
	int i,j;
	int length = GRAY_LEVELS;
	memset(num_tag, 0, sizeof(num_tag));

	//256个桶装入对应像素值得像素个数
	for ( i = 0; i < height; i++) {
		for ( j = 0; j < width; j++) {
			int grey = _cc_get_pixel(img, j, i);
			num_tag[grey & 0x000000FF]++;
		}
	}
	//数值归一化
	for ( i = 0; i < GRAY_LEVELS; i++) {
		num_tag[i] = num_tag[i] / (double) (img->size);
	}

	for ( i = 1; i < GRAY_LEVELS - 1; i++) {
		double w_pre = 0;
		double w_behind = 0;
		double u_pre = 0;
		double u_behind = 0;
		int pre;
		int behind;

		for ( pre = 0; pre < i; pre++) {
			w_pre += num_tag[pre];
			u_pre += (pre * num_tag[pre]);
		}

		if (w_pre - 0 < 0.00001) {
			continue;
		}

		u_pre = u_pre / w_pre;

		for ( behind = i; behind < length; behind++) {
			w_behind += num_tag[behind];
			u_behind += (behind * num_tag[behind]);
		}

		u_behind = u_behind / w_behind;

		g_tmp = w_pre * w_behind * (u_pre - u_behind) * (u_pre - u_behind);
		if (g_tmp > g_max) {
			threshold = i;
			g_max = g_tmp;
		}
	}
	return threshold;
}
void _CC_CALL_ _cc_convert_bnary_bitmap(_cc_image_t* dst) {
	int threshold = _calculate_binary_threshold(dst);
	//获取位图的宽
	int width = dst->width;
	//获取位图的高
	int height = dst->height;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int grey = _cc_get_pixel(dst, j, i) & 0x000000FF;
			if (grey >= threshold) {
				_cc_set_pixel(dst, j, i, 0xFFFFFFFF,0);
			} else {
				_cc_set_pixel(dst, j, i, 0xFF000000,0);
			}
		}
	}
}

void _CC_CALL_ _cc_image_to_gray(_cc_image_t *src) {
	uint32_t x = 0,y = 0;
	uint32_t gray, col;
	for (y = 0; y < src->height; ++y) {
		for (x = 0; x < src->width; ++x) {
			col = _cc_get_pixel(src, x, y);
			gray = 255 - ((colorR(col) + ((uint16_t)colorG(col) << 1) + colorB(col)) >> 2);
			_cc_set_pixel(src, x,y, _CC_R8G8B8A8(gray,gray,gray,colorA(col)), 0);
		}
	}
}

_cc_image_t* CuttingImage(_cc_image_t *src, int left, int top, int width, int height) {
	_cc_image_t *dst = _cc_init_image(CF_A8R8G8B8, width * 4, height * 4);
	if (dst == NULL) {
		return NULL;
	}
	_cc_image_resampled(dst,src,0, 0, left, top, dst->width, dst->height, width, height);

	_cc_image_to_gray(dst);
	_cc_convert_bnary_bitmap(dst);

	return dst;
}

void format(TCHAR *szLine) {
	TCHAR *startIndex = szLine;
	TCHAR *currentIndex = szLine;
	while(*currentIndex) {
		if (*currentIndex == _CC_CR_ || *currentIndex == _CC_LF_ || _cc_isspace(*currentIndex)) {
			currentIndex++;
			continue;
		}
		*startIndex++ = *currentIndex++;
	}
	*startIndex = 0;
}

BOOL _CC_CALL_ OCRProcess(TCHAR *szFileName, INT nIndex) {
	//加载资源
	TCHAR szProcessName[MAX_PATH] = TEXT("");

	_sntprintf_s(szProcessName,_cc_countof(szProcessName), _T("C:\\Program Files\\Tesseract-OCR\\tesseract.exe -l num %s %s"), szFileName, szFileName);
	PROCESS_INFORMATION ProcessInfo;
	//变量定义
	STARTUPINFO StartInfo;
	ZeroMemory(&StartInfo,sizeof(StartInfo));

	//启动进程
	StartInfo.cb = sizeof(StartInfo);
	StartInfo.dwFlags = STARTF_USESHOWWINDOW;
	StartInfo.wShowWindow = SW_HIDE;
	if (CreateProcess(NULL,szProcessName,NULL,NULL,FALSE,CREATE_DEFAULT_ERROR_MODE,NULL,NULL,&StartInfo,&ProcessInfo) == FALSE) {
		//ASSERT(FALSE);
		_CC_LOG_FMT(_CC_LOG_FLAGS_INFO_|_CC_LOG_FLAGS_ASIC_,_T("CreateProcess fail."));
		return FALSE;
	}
	//等待一个进程结束
	switch (WaitForSingleObject(ProcessInfo.hProcess,INFINITE))
	{
	case WAIT_TIMEOUT:// 等待时间超过0秒
		break;
	case WAIT_OBJECT_0:// hProcess所代表的进程在0秒内结束
	case WAIT_FAILED:// 函数调用失败，比如传递了一个无效的句柄
		break;
	}

	TCHAR szPathFile[_CC_MAX_PATH_];
	_sntprintf_s(szPathFile, _cc_countof(szPathFile), _T("%s.txt"), szFileName);

	TCHAR line[255];
	FILE *rfp = _tfopen(szPathFile, _T("r"));
	if (rfp == NULL) {
		return FALSE;
	}
	int refcount = 0;
	while(_fgetts(line, 255, rfp)) {
		tagProducts *p = (gCrawlerConfig.pProducts + refcount);
		if (line[0] == _CC_CR_ || line[0] == _CC_LF_) {
			continue;
		}
		format(line);
		if (line[0]) {
			_sntprintf(p->szField[nIndex],_cc_countof(p->szField[nIndex]), _T("[%s]"), line);
			refcount++;
		}
	}
	fclose(rfp);

	//_cc_unlink(szFileName);
	//_sntprintf_s(szProcessName,_cc_countof(szProcessName), _T("%s.txt"), szFileName);
	//_cc_unlink(szProcessName);
	return TRUE;
}

static int GetSQLString(char** rows, int index, char* val, size_t size)
{
	INT wLen = 0;
	WCHAR wBuffer[1024];
	WCHAR *wPtr = wBuffer;
	CHAR *pUTF8 = rows[index];

	INT len = MultiByteToWideChar(CP_UTF8, 0, pUTF8, -1, NULL, 0);
	if (len > 1024) {
		wLen = len;
		wPtr = (WCHAR*)_cc_malloc(sizeof(WCHAR) * (len + 1));
		if (wPtr == NULL) {
			wPtr = wBuffer;
			wLen = 0;
		}
	}

	MultiByteToWideChar(CP_UTF8, 0, pUTF8, -1, wPtr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wPtr, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, wPtr, -1, val, len, NULL, NULL);

	if(wLen > 0 && wPtr) {
		_cc_free(wPtr);
	}

	return len;
}

//1 数据库未连接
//2 查询数据库失败“products表”
//3 数据平台未配置
//4 内存分配失败
int32_t _CC_CALL_ doReadSQLProducts() {
	tchar_t szSQLString[1024];
	int32_t nProductCount = 0;
	int32_t i = 0;
	_cc_sql_result_t *resultSQL = NULL;

	if (gCrawlerConfig.pConnectSQL == NULL) {
		return 1;
	}

	if (gCrawlerConfig.pProducts) {
		free(gCrawlerConfig.pProducts);
		gCrawlerConfig.nProductCount = 0;
	}

	_sntprintf_s(szSQLString, _cc_countof(szSQLString),"select count(`id`) from `products` where `platform`=%d;",gCrawlerConfig.cbPlatform);
	resultSQL = gSQLDriver.execute(gCrawlerConfig.pConnectSQL, szSQLString, TRUE);
	if (resultSQL == NULL) {
		return 2;
	}
	if (gSQLDriver.fetch_row(resultSQL)) {
		nProductCount = gSQLDriver.get_int32(resultSQL, 0);
		gSQLDriver.free_result(gCrawlerConfig.pConnectSQL, resultSQL);
		if (nProductCount < 0) {
			return 3;
		}
	} else {
		gSQLDriver.free_result(gCrawlerConfig.pConnectSQL, resultSQL);
		return 2;
	}

	_sntprintf_s(szSQLString, _cc_countof(szSQLString),"select `id`,`name` from `products` where `platform`=%d order by `sorted` asc;",gCrawlerConfig.cbPlatform);
	resultSQL = gSQLDriver.execute(gCrawlerConfig.pConnectSQL, szSQLString, TRUE);
	if (resultSQL == NULL) {
		return 2;
	}

	gCrawlerConfig.pProducts = (tagProducts *)_cc_malloc(sizeof(tagProducts)*nProductCount);
	if (gCrawlerConfig.pProducts == NULL) {
		return 100;
	}
	gCrawlerConfig.nProductCount = nProductCount;
	ZeroMemory(gCrawlerConfig.pProducts, sizeof(tagProducts) * nProductCount);

	while (gSQLDriver.fetch_row(resultSQL)) {
		tagProducts* product = (gCrawlerConfig.pProducts + i++);
		
		product->nId = gSQLDriver.get_int32(resultSQL, 0);
		GetSQLString((char**)gSQLDriver.get_stmt(resultSQL), 1, product->szName,_cc_countof(product->szName));
		if (nProductCount <= i) {
			break;
		}
	}
	gSQLDriver.free_result(gCrawlerConfig.pConnectSQL, resultSQL);
	return 0;
}

bool_t _CC_CALL_ doConnectSQL() {
	tchar_t sqlConnect[1024];
	_sntprintf(sqlConnect, _cc_countof(sqlConnect), _T("MYSQL://%s:%s@%s:%d/%s"),
		gCrawlerConfig.szUser, gCrawlerConfig.szPassword, gCrawlerConfig.szHost, gCrawlerConfig.wPort, 
		gCrawlerConfig.szDBName);

	gCrawlerConfig.pConnectSQL = gSQLDriver.connect(sqlConnect);
	return gCrawlerConfig.pConnectSQL != NULL;
}

void _CC_CALL_ doDisconnectSQL() {
	if (gCrawlerConfig.pConnectSQL) {
		gSQLDriver.disconnect(gCrawlerConfig.pConnectSQL);
	}
}

int32_t _CC_CALL_ doWorkerInitialization(_cc_thread_t *t, pvoid_t args) {
	INT nErrorCode = 0;
	CCrawlerDialog *pWnd = (CCrawlerDialog*)args;
	pWnd->SetHint(_T("正在连接数据库..."));
	if (!doConnectSQL()) {
		pWnd->SetHint(_T("连接接数据库失败..."));
		pWnd->PostMessage(WM_MESSAGE_EVENT, EVENT_WORKER_INITIALIZED, nErrorCode);
		return 1;
	}

	pWnd->SetHint(_T("正在读取数据配置..."));
	nErrorCode = doReadSQLProducts();
	switch(nErrorCode) {
	case 1:
		pWnd->SetHint(_T("数据库未连接"));
		break;
	case 2:
		pWnd->SetHint(_T("查询数据库失败“products表”"));
		break;
	case 3:
		pWnd->SetHint(_T("数据平台未配置..."));
		break;
	case 4:
		pWnd->SetHint(_T("内存分配失败..."));
		break;
	}
	
	pWnd->SetHint(_T("完成"));
	pWnd->PostMessage(WM_MESSAGE_EVENT, EVENT_WORKER_INITIALIZED, nErrorCode);
	return 1;
}

int32_t _CC_CALL_ doStartingWorker(_cc_thread_t *t, pvoid_t args) {
	CScreenshots Screenshot;
	CCrawlerDialog *pWnd = (CCrawlerDialog*)args;

	clock_t start, end;

	_cc_init_udp_log(_T("127.0.0.1"), _T("8600"));

	while (pWnd->IsStarting()) {
		start = clock();
		
		if (!Screenshot.Screenshot(pWnd->GetFromWindow())) {
			pWnd->SetHint(_T("截图失败"));
			pWnd->PostMessage(WM_MESSAGE_EVENT, EVENT_WORKER_CLOSEED, 0);
			return 1;
		}
		
		_cc_image_t *src = Screenshot.toImage();
		INT nWidth = 0;
		INT nHeight = gCrawlerScreenshot.rcBox.bottom - gCrawlerScreenshot.rcBox.top;
		if (src) {
			for (int i = 0; i < MAX_CRAWLER_DATA_BOX; i++) {
				_cc_image_t *dst = CuttingImage(src, gCrawlerScreenshot.rcBox.left + nWidth, gCrawlerScreenshot.rcBox.top, gCrawlerScreenshot.nBoxWidth[i], nHeight);
				nWidth += gCrawlerScreenshot.nBoxWidth[i];
				if (dst) {
					_cc_write_JPG(pWnd->GetScreenshotPathFile(i), dst, 100);
					_cc_destroy_image(&dst);
				}
				OCRProcess(pWnd->GetScreenshotPathFile(i), i);
			}
		}
		_cc_destroy_image(&src);
		_cc_sleep(100);

		end = clock();
		for(INT i = 0; i < gCrawlerConfig.nProductCount; i++) {
			tagProducts *p = (gCrawlerConfig.pProducts + i);
			_CC_LOG_FMT(_CC_LOG_FLAGS_INFO_|_CC_LOG_FLAGS_ASIC_,_T("%s : %s,%s,%s"), p->szName, p->szField[0], p->szField[1], p->szField[2]);

			p->szField[0][0] = 0;
			p->szField[1][0] = 0;
			p->szField[2][0] = 0;
		}
		_CC_LOG_FMT(_CC_LOG_FLAGS_INFO_|_CC_LOG_FLAGS_ASIC_,_T("--%f---------------------------"),(double)(end - start) / CLOCKS_PER_SEC);
	}

	pWnd->PostMessage(WM_MESSAGE_EVENT, EVENT_WORKER_CLOSEED, 0);
	return 1;
}
