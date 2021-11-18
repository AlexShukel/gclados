#ifndef GCLADOS_TEST_SUITE_H
#define GCLADOS_TEST_SUITE_H

#include <stdbool.h>

#include "test.h"

typedef enum {
    PTF_PASS,
    PTF_FAILED,
    PTF_WAITING,
    PTF_RUNNING,
    PTF_SKIP,
} GcladosTestSuiteStatus;

typedef struct {
    const char *testSuiteName;
    GcladosTest *tests;
    size_t testCount;
    size_t completedTestCount;
    GcladosTestSuiteStatus status;
} GcladosTestSuite;

GcladosTestSuite gcladosCreateTestSuite(const char *testSuiteName, GcladosTest *tests, size_t testCount);
void gcladosPrintSuite(GcladosTestSuite suite, bool minified);
bool gcladosRunNextTest(GcladosTestSuite *suite);
void gcladosFreeTestSuite(GcladosTestSuite *suite);

#endif
