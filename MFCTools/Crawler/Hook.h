#ifndef _CC_HOOK_H_INCLUDED_
#define _CC_HOOK_H_INCLUDED_

//////////////////////////////////////////////////
#include <cc/rbtree.h>
#include <windows.h>
#include <stdio.h>
#include <Psapi.h>
#include <tlhelp32.h>

typedef struct _tagProcessModule {
	tchar_t	szModule[MAX_MODULE_NAME32 + 1]; 
	DWORD_PTR dwBaseAddr;	// Base address of module in th32ProcessID's context
	DWORD dwBaseSize;	// Size in bytes of module starting at modBaseAddr
	HICON hIcon;
	_cc_rb_node_t node;
}tagProcessModule;

///////////////////////////////////////////////////
// ��    ��: EnumProcessModule
// ��    ��: ��ȡָ������ָ��ģ���ַ
// ��    ��:
//    DWORD dwProcessID -- ����PID
// �� �� ֵ: ��
// ��    ע:
//  
///////////////////////////////////////////////////
void _CC_CALL_ EnumProcessModule(DWORD dwProcessID);
///////////////////////////////////////////////////
// ��    ��: EnumProcess
// ��    ��: ����ָ������
// ��    ��:
//    bool_t func(LPPROCESSENTRY32) -- �ص�����,����TRUE �ж�ѭ��
// �� �� ֵ: ��
// ��    ע:
//         
///////////////////////////////////////////////////
void _CC_CALL_ EnumProcess(bool_t (_CC_CALL_ *func)(LPPROCESSENTRY32,pvoid_t ), pvoid_t args);
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
bool_t _CC_CALL_ EnableDebugPrivileges(void);

_cc_rb_node_t* _CC_CALL_ FirstProcessModule(void);
tagProcessModule* _CC_CALL_ FindProcessModule(const tchar_t *szModuleName);

void _CC_CALL_ QuitHook(void);
#endif