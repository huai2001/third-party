#pragma once

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
#endif

#include <afx.h>
#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <cc/core.h>
// Windows ͷ�ļ�:
#include <windows.h>

//////////////////////////////////////////////////////////////////////////////////
//���ó���

//��Ч��ֵ
#define INVALID_BYTE				((BYTE)(0xFF))						//��Ч��ֵ
#define INVALID_WORD				((WORD)(0xFFFF))					//��Ч��ֵ
#define INVALID_DWORD				((DWORD)(0xFFFFFFFF))				//��Ч��ֵ

//////////////////////////////////////////////////////////////////////////////////
//����ά��
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))

//�ӿ��ͷ�
#define SafeRelease(pObject) { if (pObject!=NULL) { pObject->Release(); pObject=NULL; } }

//ɾ��ָ��
#define SafeDelete(pData) { try { delete pData; } catch (...) { ASSERT(FALSE); } pData=NULL; } 

//ɾ��ָ��
#define SafeFree(pData) { try { free(pData); } catch (...) { ASSERT(FALSE); } pData=NULL; } 

//�رվ��
#define SafeCloseHandle(hHandle) { if (hHandle!=NULL) { CloseHandle(hHandle); hHandle=NULL; } }

//ɾ������
#define SafeDeleteArray(pData) { try { delete [] pData; } catch (...) { ASSERT(FALSE); } pData=NULL; } 

//ͼƬ�ؼ�
#import "ImageOle.dll" named_guids

//////////////////////////////////////////////////////////////////////////////////
//���Ӵ���
#ifndef WIN64
//�����ļ�
#include "Other\SkinSB.h"
#include "Other\Detours.h"

#pragma comment (lib,"Other/SkinSB.lib")
#pragma comment (lib,"Other/Detours.lib")
#endif

//////////////////////////////////////////////////////////////////////////////////
