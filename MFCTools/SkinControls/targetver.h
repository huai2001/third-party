#pragma once

#if defined(_MSC_VER)
	/* Microsoft Visual C++
	//
	// Must remain the last #elif since some other vendors (Metrowerks, for example)
	// also #define _MSC_VER
	// Compiler version defines: VC6.0 : 1200, VC7.0 : 1300, VC7.1 : 1310, VC8.0 : 1400, VC
	*/
	#ifndef _CRT_SECURE_NO_DEPRECATE
		#pragma warning(disable:4996)
	#endif

	#if _MSC_VER >= 1600// VC 2010
		#pragma warning(disable: 4819)
		#pragma warning(disable: 4996)
	#elif _MSC_VER >= 1500 // VC2008
		#pragma warning(disable: 4819)
		#pragma warning(disable: 4996)
	#elif _MSC_VER >= 1400 // VC2005
		#pragma warning(disable: 4819)
		#pragma warning(disable: 4996)
	#elif _MSC_VER >= 1310 // VC2003
	#elif _MSC_VER >= 1300 // VC7.0
	#elif _MSC_VER >= 1200 // VC6
	#else
		
	#endif
#endif

// ���� SDKDDKVer.h ��������õ���߰汾�� Windows ƽ̨��

// ���ҪΪ��ǰ�� Windows ƽ̨����Ӧ�ó�������� WinSDKVer.h������
// WIN32_WINNT ������ΪҪ֧�ֵ�ƽ̨��Ȼ���ٰ��� SDKDDKVer.h��

#include <SDKDDKVer.h>