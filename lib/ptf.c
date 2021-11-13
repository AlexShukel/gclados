#include "ptf.h"

#include <stdio.h>
#include <stdlib.h>

void ptfDrawSuites(struct PtfTestSuite* suites, size_t count, bool minified) {
    // Clear screen
    printf("\033c");

    for(int i = 0; i < count; i++) {
        printSuite(suites[i], minified);
    }
}

void runPtfTestSuites(struct PtfTestSuite* suites, size_t count) {
    size_t completedTestSuites = 0;

    while(completedTestSuites != count) {
        suites[completedTestSuites].status = PTF_RUNNING;

        ptfDrawSuites(suites, count, true);
        runTestSuite();

        ++completedTestSuites;
    }
}
