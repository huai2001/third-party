#ifndef _CC_HOOK_H_INCLUDED_
#define _CC_HOOK_H_INCLUDED_

//////////////////////////////////////////////////
#include <cc/rbtree.h>
#include <windows.h>
#include <stdio.h>
#include <Psapi.h>
#include <tlhelp32.h>

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
BOOL  EnableDebugPrivileges(void);
///////////////////////////////////////////////////
// 函    数: InjectDLL
// 描    述: 远程线程注入DLL
// 参    数: 
//			dwProcessID 进程ID
//			szPath 需要注入的DLL
// 返 回 值:
//    int -- 成功返回 TRUE
//           失败返回 FALSE
// 备    注:
//         
///////////////////////////////////////////////////
BOOL  InjectDLL(DWORD dwProcessID, TCHAR *szPath);
///////////////////////////////////////////////////
// 函    数: UnInjectDLL
// 描    述: 远程线程注入DLL
// 参    数: 
//			dwProcessID 进程ID
//			dwBaseAddress 需要卸载的DLL基址
// 返 回 值:
//    int -- 成功返回 TRUE
//           失败返回 FALSE
// 备    注:
//         
///////////////////////////////////////////////////
BOOL _CC_CALL_ unInjectDLL(DWORD dwProcessID, DWORD_PTR dwBaseAddress);
#endif