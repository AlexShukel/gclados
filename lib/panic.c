#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)
#define OS_WINDOWS 1
#elif defined(linux) || defined(unix) || defined(__unix) || defined(__unix__)
#define OS_LINUX 1
#else
#warning Your OS is not supported by ptf framework. This could lead to unexpected errors.
#endif

#include "panic.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef OS_WINDOWS
// #include <windows.h>
// #include <imagehlp.h>
#endif

#ifdef OS_LINUX
#include <execinfo.h>
#endif

void gcladosPrintBacktrace(FILE *file);

void gcladosPanic(char *message, int exitCode) {
    fflush(stdout);
    fprintf(stderr, "GcLaDOS panicked: %s\n", message);
    gcladosPrintBacktrace(stderr);
    fflush(stderr);
    exit(exitCode);
}

#if defined(OS_WINDOWS)
void gcladosPrintBacktrace(FILE *file) {
    /* TODO: implement backtrace for windows
    SymInitialize(GetCurrentProcess(), NULL, TRUE);
    CONTEXT context;
    RtlCaptureContext(&context);

    DWORD machineType;
    STACKFRAME stackFrame;
    memset(&stackFrame, 0, sizeof(STACKFRAME));

    #ifdef _M_IX86
        machineType = IMAGE_FILE_MACHINE_I386;
        stackFrame.AddrPC.Offset = context.Eip;
        stackFrame.AddrFrame.Offset = context.Ebp;
        stackFrame.AddrStack.Offset = context.Esp;
    #elif _M_X64
        machineType = IMAGE_FILE_MACHINE_AMD64;
        stackFrame.AddrPC.Offset = context.Rip;
        stackFrame.AddrFrame.Offset = context.Rsp;
        stackFrame.AddrStack.Offset = context.Rsp;
    #elif _M_IA64
        machineType = IMAGE_FILE_MACHINE_IA64;
        stackFrame.AddrPC.Offset = context.StIIP;
        stackFrame.AddrFrame.Offset = context.IntSp;
        stackFrame.AddrStack.Offset = context.IntSp;
        stackFrame.AddrBStore.Offset = context.RsBSP;
        stackFrame.AddrBStore.Mode = AddrModeFlat;
    #else
    #warning Stack trace is not supported on your machine.
        return;
    #endif

    while(StackWalk(machineType,
                    GetCurrentProcess(),
                    GetCurrentThread(),
                    &stackFrame,
                    &context,
                    NULL,
                    SymFunctionTableAccess,
                    SymGetModuleBase,
                    NULL)) {

        PSYMBOL_INFO Symbol;
        IMAGEHLP_MODULE module = { 0 };
        module.SizeOfStruct = sizeof(module);
        SymGetModuleInfo(GetCurrentProcess(), stackFrame.AddrPC.Offset, &module);

        if(SymFromAddr(GetCurrentProcess(), stackFrame.AddrPC.Offset, NULL, Symbol)) {
            fprintf(file, "%s!%s\n", module.ModuleName, Symbol->Name);
        } else {
            fprintf(file, "%s!%s\n", module.ModuleName, "unknown");
        }
    }

    SymCleanup(GetCurrentProcess());
     */
}
#elif defined(OS_LINUX)
void gcladosPrintBacktrace(FILE *file) {
    void *backtraceBuffer[1024];
    int backtraceSize = backtrace(backtraceBuffer, 1024);
    char **backtraceSymbols = backtrace_symbols(backtraceBuffer, backtraceSize);

    for(int i = 1; i < backtraceSize; ++i) {
        fprintf(stderr, "%s\n", backtraceSymbols[i]);
    }
}
#elif
void gcladosPrintBacktrace(FILE *file) {
    // Do nothing - system is not supported.
}
#endif

#undef OS_WINDOWS
#undef OS_LINUX
