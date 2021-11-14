#ifndef __PTF_TEST_SUITE_H__
#define __PTF_TEST_SUITE_H__

#include <stdlib.h>
#include <stdbool.h>
#include "panic.h"
#include "colors.h"
#include "test.h"

enum PtfTestSuiteStatus {
    PTF_PASS,
    PTF_FAILED,
    PTF_WAITING,
    PTF_RUNNING,
    PTF_SKIP,
};

struct PtfTestSuite {
    const char* testSuiteName;
    struct PtfTest* tests;
    size_t testCount;
    size_t completedTestCount;
    enum PtfTestSuiteStatus status;
};

struct PtfTestSuite createPtfTestSuite(const char* testSuiteName, struct PtfTest* tests, size_t testCount);
void printSuite(struct PtfTestSuite suite, bool minified);
bool ptfRunNextTest(struct PtfTestSuite *suite);
void ptfFreeTestSuite(struct PtfTestSuite *suite);

#endif
