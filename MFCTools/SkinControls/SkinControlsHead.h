#ifndef SKIN_CONTROL_HEAD_FILE
#define SKIN_CONTROL_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//�����ļ�

//MFC �ļ�
#include <GdiPlus.h>
#include <AtlImage.h>

//�����ռ�
using namespace Gdiplus;
//////////////////////////////////////////////////////////////////////////////////
//��������

//��������
#ifndef SKIN_CONTROLS_CLASS
	#ifdef  SKINCONTROLS_EXPORTS
		#define SKIN_CONTROLS_CLASS _declspec(dllexport)
	#else
		#define SKIN_CONTROLS_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifndef _DEBUG
	#define SKIN_CONTROLS_DLL_NAME	TEXT("SkinControls.dll")			//�������
#else
	#define SKIN_CONTROLS_DLL_NAME	TEXT("SkinControlsD.dll")		//�������
#endif

//////////////////////////////////////////////////////////////////////////////////
//�����ļ�

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
