#ifndef SKIN_CONTROL_HEAD_FILE
#define SKIN_CONTROL_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//包含文件

//MFC 文件
#include <GdiPlus.h>
#include <AtlImage.h>

//命名空间
using namespace Gdiplus;
//////////////////////////////////////////////////////////////////////////////////
//公共定义

//导出定义
#ifndef SKIN_CONTROLS_CLASS
	#ifdef  SKINCONTROLS_EXPORTS
		#define SKIN_CONTROLS_CLASS _declspec(dllexport)
	#else
		#define SKIN_CONTROLS_CLASS _declspec(dllimport)
	#endif
#endif

//模块定义
#ifndef _DEBUG
	#define SKIN_CONTROLS_DLL_NAME	TEXT("SkinControls.dll")			//组件名字
#else
	#define SKIN_CONTROLS_DLL_NAME	TEXT("SkinControlsD.dll")		//组件名字
#endif

//////////////////////////////////////////////////////////////////////////////////
//导出文件

#include "BitImage.h"
#include "GIFImage.h"
#include "PngImage.h"

#ifndef SKINCONTROLS_EXPORTS
	#include "SkinEdit.h"
	#include "SkinMenu.h"
	#include "SkinDialog.h"
	#include "SkinDpi.h"
	#include "SkinButton.h"
	#include "SkinTabCtrl.h"
	#include "SkinSplitter.h"
	#include "SkinComboBox.h"
	#include "SkinEncircle.h"
	#include "SkinListCtrl.h"
	#include "SkinRichEdit.h"
	#include "SkinScrollBar.h"
	#include "SkinHyperLink.h"
	#include "SkinRenderManager.h"
	#include "SkinResourceManager.h"
	#include "SkinToolBar.h"
	#include "SystemTrayicon.h"
#endif

//////////////////////////////////////////////////////////////////////////////////

#endif
