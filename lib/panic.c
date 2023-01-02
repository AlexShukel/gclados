// Author: Artiom Tretjakovas
// Description: This file contains implementation of panic function.

// These macros required, because panic function implementation differs for Windows and Linux OS.
#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(linux) || defined(unix) || defined(__unix) || defined(__unix__)
#define SUPPORTED_OS 1
#else
#warning Your OS is not supported by ptf framework. This could lead to unexpected errors.
#endif

#include "panic.h"

#include <stdio.h>
#include <stdlib.h>

void gcladosPrintBacktrace(FILE *file);

void gcladosPanic(const char *message, int exitCode) {
    fflush(stdout);
    fprintf(stderr, "GcLaDOS panicked: %s\n", message);
    gcladosPrintBacktrace(stderr);
    fflush(stderr);
    exit(exitCode);
}

void gcladosPrintBacktrace(FILE *file) {
    // Do nothing - system is not supported.
}

#undef SUPPORTED_OS
