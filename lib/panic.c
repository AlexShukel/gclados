#include "panic.h"

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>

void ptfPanic(char* message) {
    fflush(stdout);
    fflush(stderr);
    fprintf(stderr, "Ptf panicked: %s\n", message);
    void *backtraceBuffer[1024];

    int backtraceSize = backtrace(backtraceBuffer, 1024);
    char** backtraceSymbols = backtrace_symbols(backtraceBuffer, backtraceSize);

    for(int i = 1; i < backtraceSize; ++i) {
        fprintf(stderr, "%s\n", backtraceSymbols[i]);
    }

    exit(1);
}
