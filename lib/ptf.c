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

    struct PtfDynamicArray results = createPtfDynamicArray(sizeof(struct PtfStatementResult));

    while(completedTestSuites < count) {
        suites[completedTestSuites].status = PTF_RUNNING;
        ptfDrawSuites(suites, count, true);
        if(ptfRunNextTest(&suites[completedTestSuites], &results)) {
            bool failed = false;
            for(size_t i = 0; i < results.length; ++i) {
                struct PtfStatementResult* statementResult = ptfGet(&results, i);

                if(statementResult->pass == false) {
                    failed = true;
                }
            }

            suites[completedTestSuites].status = failed ? PTF_FAILED : PTF_PASS;

            ++completedTestSuites;
        }
    }

    ptfDrawSuites(suites, count, false);
}
