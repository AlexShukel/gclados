#include "testSuite.h"

#include <stdio.h>

#include "ioutils.h"
#include "panic.h"
#include "colors.h"

GcladosTestSuite gcladosCreateTestSuite(const char *testSuiteName, GcladosTest *tests, size_t testCount) {
    GcladosTestSuite suite = {
            .testSuiteName = testSuiteName,
            .tests = tests,
            .testCount = testCount,
            .status = PTF_WAITING,
            .completedTestCount = 0,
    };

    return suite;
}

bool gcladosRunNextTest(GcladosTestSuite *suite) {
    if(suite->completedTestCount >= suite->testCount) {
        gcladosPanic("Trying to run more tests than actually exist.", EXIT_FAILURE);
    }

    gcladosRunTest(&suite->tests[suite->completedTestCount]);

    suite->completedTestCount += 1;

    return suite->completedTestCount == suite->testCount;
}

void gcladosPrintSuite(GcladosTestSuite suite, bool minified) {
    char* status;

    switch(suite.status) {
        case PTF_WAITING:
            status = gcladosColors.applyFlags(" WAIT ", gcladosColors.createFlags(2, gcladosColors.foregroundColor(GCLADOS_YELLOW), gcladosColors.framed()));
            break;
        case PTF_RUNNING:
            status = gcladosColors.applyFlags(" RUNS ", gcladosColors.createFlags(2, gcladosColors.foregroundColor(GCLADOS_CYAN), gcladosColors.framed()));
            break;
        case PTF_FAILED:
            status = gcladosColors.applyFlags(" FAIL ", gcladosColors.createFlags(2, gcladosColors.foregroundColor(GCLADOS_BLACK), gcladosColors.backgroundColor(GCLADOS_RED)));
            break;
        case PTF_PASS:
            status = gcladosColors.applyFlags(" PASS ", gcladosColors.createFlags(1, gcladosColors.backgroundColor(GCLADOS_GREEN)));
            break;
        case PTF_SKIP:
            status = gcladosColors.applyFlags(" SKIP ", gcladosColors.createFlags(1, gcladosColors.backgroundColor(GCLADOS_YELLOW)));
            break;
    }

    printf("%s %s", status, suite.testSuiteName);

    free(status);

    if(suite.status == PTF_RUNNING) {
        gcladosPrintProgress(stdout, (double) (suite.completedTestCount + 1) / (double) suite.testCount, 5);
    }

    printf("\n");

    if(!minified && suite.status == PTF_FAILED) {
        for(int i = 0; i < suite.testCount; ++i) {
            gcladosPrintTest(suite.tests[i]);
        }
    }
}

void gcladosFreeTestSuite(GcladosTestSuite *suite) {
    for(size_t i = 0; i < suite->testCount; ++i) {
        gcladosFreeTest(&suite->tests[i]);
    }

    free((void*) suite->testSuiteName);
    free(suite->tests);
    free(suite);
}
