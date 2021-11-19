#include "gclados.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

void gcladosDrawSuites(GcladosTestSuite *suites, size_t count, bool minified) {
    fflush(stdout);
    // Clear screen
    printf("\033c");

    for(int i = 0; i < count; i++) {
        gcladosPrintSuite(suites[i], minified);
    }
}

int gcladosRunTestSuites(GcladosTestSuite *suites, size_t count) {
    size_t completedTestSuites = 0;

    while(completedTestSuites < count) {
        suites[completedTestSuites].status = PTF_RUNNING;
        gcladosDrawSuites(suites, count, true);
        if(gcladosRunNextTest(&suites[completedTestSuites])) {
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

    gcladosDrawSuites(suites, count, false);

    for(size_t i = 0; i < count; ++i) {
        gcladosFreeTestSuite(&suites[i]);
    }

    return EBUSY;
}
