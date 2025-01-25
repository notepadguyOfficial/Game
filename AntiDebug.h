#pragma once
#ifndef ANTI_DEBUG_H
#define ANTI_DEBUG_H

#include <Windows.h>
#include <iostream>

class AntiDebug{
    public:
        struct DbgRemoteBreakPatch
        {
            WORD push_0;
            BYTE push;
            DWORD CurrentProcessHandle;
            BYTE mov_eax;
            DWORD TerminateProcess;
            WORD call_eax;
        };

        AntiDebug();

        bool SearchSoftwareBreakPoint(PVOID mAddress) {
            PBYTE pBytes = (PBYTE)mAddress;
            for(SIZE_T i = 0; ; i++) 
                if(pBytes[i] == 0xCC)
                    return true;

            return false;
        }

        bool SearchHardwareBreakPoint(HANDLE tHandle) {
            CONTEXT ctx;
            ZeroMemory(&ctx, sizeof(ctx));
            ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;

            GetThreadContext(tHandle, &ctx);
            return (ctx.Dr0 != 0 || ctx.Dr1 != 0 || ctx.Dr2 != 0 || ctx.Dr3 != 0) ? true : false;
        }

        bool DbgPresent() {
            return isDbgPresent();
        }

        bool AntiDbgBreakPoint() {
            FARPROC pDbgBreakPoint = GetProcAddress(GetModuleHandleA("ntdll.dll"), "DbgBreakPoint");
            
            if(!pDbgBreakPoint)
                return false;

            DWORD oldProtect;
            VirtualProtect(pDbgBreakPoint, 1, PAGE_EXECUTE_READWRITE, &oldProtect);
            *(PBYTE)pDbgBreakPoint = (BYTE)0xC3;

            return true;
        }

        bool antiDbgUiRemoteBreakin() {
            FARPROC pDbgUiRemoteBreakin = GetProcAddress(GetModuleHandleA("ntdll.dll"), "DbgUiRemoteBreakin");
            FARPROC pTerminateProcess = GetProcAddress(GetModuleHandleA("kernel32.dll"), "TerminateProcess");

            DbgUiRemoteBreakinPatch patch = { 0 };
            patch.push_0 = 0x6A00;
            patch.push = 0x68;
            patch.CurrentProcessHandle = 0xFFFFFFFF;
            patch.mov_eax = 0xB8;
            patch.TerminateProcess = (DWORD)pTerminateProcess;
            patch.call_eax = 0xFFD0;

            DWORD oldProtect;
            VirtualProtect(pDbgUiRemoteBreakin, sizeof(DbgUiRemoteBreakinPatch), PAGE_READWRITE, &oldProtect);

            memcpy(pDbgUiRemoteBreakin, &patch, sizeof(DbgUiRemoteBreakinPatch));
            VirtualProtect(pDbgUiRemoteBreakin, sizeof(DbgUiRemoteBreakinPatch), oldProtect, &oldProtect);
            
            return true;
        }

        /**
         * @brief Checks if a debugger is present in a remote process.
         *
         * This function uses the Windows API function CheckRemoteDebuggerPresent
         * to determine if a debugger is attached to the specified process.
         *
         * @param hProcess A handle to the process to be checked for a debugger.
         * @return true if a debugger is present in the remote process, false otherwise.
         */
        bool isDebuggerPresentInRemoteProcess(HANDLE hProcess) {
            bool isDebuggerPresent = false;
            CheckRemoteDebuggerPresent(hProcess, &isDebuggerPresent);

            return isDebuggerPresent;
        }


        bool patchDebuggingFunctions() {
            antiDbgBreakPoint();
            antiDbgUiRemoteBreakin();
        }
};

extern AntiDebug& adbg;

#endif