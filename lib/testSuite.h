#ifndef GCLADOS_TEST_SUITE_H
#define GCLADOS_TEST_SUITE_H

#include <stdbool.h>

#include "test.h"

typedef enum {
    GCLADOS_PASS,
    GCLADOS_FAILED,
    GCLADOS_WAITING,
    GCLADOS_RUNNING,
    GCLADOS_SKIP,
} GcladosTestSuiteStatus;

typedef struct {
    const char *testSuiteName;
    GcladosTest *tests;
    size_t testCount;
} GcladosTestSuite;

typedef struct {
    GcladosTestSuiteStatus status;
    GcladosTestResult *testResults;
    size_t testResultCount;
    size_t completedTestCount;
} GcladosTestSuiteAccumulatedResult;

GcladosTestSuiteAccumulatedResult gcladosInitializeTestSuiteResult(GcladosTestSuite suite);
void gcladosCompleteTestSuiteResult(GcladosTestSuiteAccumulatedResult *result);
GcladosTestSuite gcladosCreateTestSuite(const char *testSuiteName, GcladosTest *tests, size_t testCount);
void gcladosPrintSuite(GcladosTestSuite suite, GcladosTestSuiteAccumulatedResult state);
bool gcladosRunNextTest(GcladosTestSuite suite, GcladosTestSuiteAccumulatedResult *state);
void gcladosFreeTestSuiteResult(GcladosTestSuiteAccumulatedResult testSuiteResult);

#endif
