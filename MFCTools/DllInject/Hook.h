#ifndef _CC_HOOK_H_INCLUDED_
#define _CC_HOOK_H_INCLUDED_

//////////////////////////////////////////////////
#include <cc/rbtree.h>
#include <windows.h>
#include <stdio.h>
#include <Psapi.h>
#include <tlhelp32.h>

///////////////////////////////////////////////////
// ��    ��: EnableDebugPrivileges
// ��    ��: ��������Ȩ��
// ��    ��: ��
// �� �� ֵ:
//    int -- �ɹ����� TRUE
//           ʧ�ܷ��� FALSE
// ��    ע:
//         
///////////////////////////////////////////////////
BOOL  EnableDebugPrivileges(void);
///////////////////////////////////////////////////
// ��    ��: InjectDLL
// ��    ��: Զ���߳�ע��DLL
// ��    ��: 
//			dwProcessID ����ID
//			szPath ��Ҫע���DLL
// �� �� ֵ:
//    int -- �ɹ����� TRUE
//           ʧ�ܷ��� FALSE
// ��    ע:
//         
///////////////////////////////////////////////////
BOOL  InjectDLL(DWORD dwProcessID, TCHAR *szPath);
///////////////////////////////////////////////////
// ��    ��: UnInjectDLL
// ��    ��: Զ���߳�ע��DLL
// ��    ��: 
//			dwProcessID ����ID
//			dwBaseAddress ��Ҫж�ص�DLL��ַ
// �� �� ֵ:
//    int -- �ɹ����� TRUE
//           ʧ�ܷ��� FALSE
// ��    ע:
//         
///////////////////////////////////////////////////
BOOL _CC_CALL_ unInjectDLL(DWORD dwProcessID, DWORD_PTR dwBaseAddress);
#endif