#include "testSuite.h"
#include <stdio.h>

struct PtfTestSuite createPtfTestSuite(const char* testSuiteName, struct PtfTest* tests, size_t testCount) {
    struct PtfTestSuite suite = {
            .testSuiteName = testSuiteName,
            .tests = tests,
            .testCount = testCount,
            .status = PTF_RUNNING,
    };

    return suite;
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

    printf("%s %s\n", status, suite.testSuiteName);

    free(status);
}
