#include "testSuite.h"

#include <stdio.h>

#include "ioutils.h"

struct PtfTestSuite createPtfTestSuite(const char* testSuiteName, struct PtfTest* tests, size_t testCount) {
    struct PtfTestSuite suite = {
            .testSuiteName = testSuiteName,
            .tests = tests,
            .testCount = testCount,
            .status = PTF_WAITING,
            .completedTestCount = 0,
    };

    return suite;
}

bool ptfRunNextTest(struct PtfTestSuite *suite, struct PtfDynamicArray* results) {
    if(suite->completedTestCount >= suite->testCount) {
        // TODO: add panic here
    }

    ptfRunTest(suite->tests[suite->completedTestCount], results);
    suite->completedTestCount += 1;

    return suite->completedTestCount == suite->testCount;
}

void printSuite(struct PtfTestSuite suite, bool minified) {
    char* status;

    switch(suite.status) {
        case PTF_WAITING:
            status = ptfColors.applyFlags(" WAIT ", ptfColors.createFlags(2, ptfColors.foregroundColor(PTF_YELLOW), ptfColors.framed()));
            break;
        case PTF_RUNNING:
            status = ptfColors.applyFlags(" RUNS ", ptfColors.createFlags(2, ptfColors.foregroundColor(PTF_CYAN), ptfColors.framed()));
            break;
        case PTF_FAILED:
            status = ptfColors.applyFlags(" FAIL ", ptfColors.createFlags(2, ptfColors.foregroundColor(PTF_BLACK), ptfColors.backgroundColor(PTF_RED)));
            break;
        case PTF_PASS:
            status = ptfColors.applyFlags(" PASS ", ptfColors.createFlags(1, ptfColors.backgroundColor(PTF_GREEN)));
            break;
        case PTF_SKIP:
            status = ptfColors.applyFlags(" SKIP ", ptfColors.createFlags(1, ptfColors.backgroundColor(PTF_YELLOW)));
            break;
    }

    printf("%s %s", status, suite.testSuiteName);

    if(suite.status == PTF_RUNNING) {
        ptfPrintProgress(stdout, (double) (suite.completedTestCount + 1) / (double) suite.testCount, 5);
    }

    printf("\n");

    free(status);
}
