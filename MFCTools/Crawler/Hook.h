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
// 函    数: EnumProcessModule
// 描    述: 获取指定进程指定模块基址
// 参    数:
//    DWORD dwProcessID -- 进程PID
// 返 回 值: 无
// 备    注:
//  
///////////////////////////////////////////////////
void _CC_CALL_ EnumProcessModule(DWORD dwProcessID);
///////////////////////////////////////////////////
// 函    数: EnumProcess
// 描    述: 查找指定进程
// 参    数:
//    bool_t func(LPPROCESSENTRY32) -- 回调函数,返回TRUE 中断循环
// 返 回 值: 无
// 备    注:
//         
///////////////////////////////////////////////////
void _CC_CALL_ EnumProcess(bool_t (_CC_CALL_ *func)(LPPROCESSENTRY32,pvoid_t ), pvoid_t args);
///////////////////////////////////////////////////
// 函    数: EnableDebugPrivileges
// 描    述: 提升进程权限
// 参    数: 无
// 返 回 值:
//    int -- 成功返回 TRUE
//           失败返回 FALSE
// 备    注:
//         
///////////////////////////////////////////////////
bool_t _CC_CALL_ EnableDebugPrivileges(void);

_cc_rb_node_t* _CC_CALL_ FirstProcessModule(void);
tagProcessModule* _CC_CALL_ FindProcessModule(const tchar_t *szModuleName);

void _CC_CALL_ QuitHook(void);
#endif