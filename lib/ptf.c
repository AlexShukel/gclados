#include "ptf.h"

#include <stdio.h>
#include <stdlib.h>

void ptfDrawSuites(struct PtfTestSuite* suites, size_t count, bool minified) {
    fflush(stdout);
    // Clear screen
    printf("\033c");

    for(int i = 0; i < count; i++) {
        printSuite(suites[i], minified);
    }
}

void runPtfTestSuites(struct PtfTestSuite* suites, size_t count) {
    size_t completedTestSuites = 0;

    while(completedTestSuites < count) {
        suites[completedTestSuites].status = PTF_RUNNING;
        ptfDrawSuites(suites, count, true);
        if(ptfRunNextTest(&suites[completedTestSuites])) {
            bool pass = true;
            for(size_t i = 0; i < suites[completedTestSuites].testCount; ++i) {
                if(suites[completedTestSuites].tests[i].pass == false) {
                    pass = false;
                    break;
                }
            }

            suites[completedTestSuites].status = pass ? PTF_PASS : PTF_FAILED;

            ++completedTestSuites;
        }
    }

    ptfDrawSuites(suites, count, false);

    for(size_t i = 0; i < count; ++i) {
        ptfFreeTestSuite(&suites[i]);
    }
}
