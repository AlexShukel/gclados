// Author: Artiom Tretjakovas
// Description: This file contains implementation of panic function.

// These macros required, because panic function implementation differs for Windows and Linux OS.
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

#ifdef OS_LINUX
#include <execinfo.h>
#endif

void gcladosPrintBacktrace(FILE *file);

void gcladosPanic(const char *message, int exitCode) {
    fflush(stdout);
    fprintf(stderr, "GcLaDOS panicked: %s\n", message);
    gcladosPrintBacktrace(stderr);
    fflush(stderr);
    exit(exitCode);
}

#ifdef OS_LINUX
void gcladosPrintBacktrace(FILE *file) {
    void *backtraceBuffer[1024];
    int backtraceSize = backtrace(backtraceBuffer, 1024);
    char **backtraceSymbols = backtrace_symbols(backtraceBuffer, backtraceSize);

    for(int i = 1; i < backtraceSize; ++i) {
        fprintf(stderr, "%s\n", backtraceSymbols[i]);
    }
}
#else
void gcladosPrintBacktrace(FILE *file) {
    // Do nothing - system is not supported.
}
#endif

#undef OS_WINDOWS
#undef OS_LINUX
