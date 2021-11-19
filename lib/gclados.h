#ifndef GCLADOS_LIB_H
#define GCLADOS_LIB_H

#include "panic.h"
#include "predicateNamespace.h"
#include "test.h"
#include "testSuite.h"
#include "statement.h"
#include "colors.h"

int gcladosRunTestSuites(GcladosTestSuite *suites, size_t count);

#define GCLADOS_CONCAT(a, b) GCLADOS_CONCAT_INNER(a, b)
#define GCLADOS_CONCAT_INNER(a, b) a ## b
#define GCLADOS_ENSURE_INNER(varName, value, predicate) typeof(value) (varName) = value; gcladosCreateStatement(__LINE__, __FILE__, &(varName), predicate)

#define GCLADOS_TEST_PREFIX __gcladosTest_
#define ensure(value, predicate) GCLADOS_ENSURE_INNER(GCLADOS_CONCAT(__gcladosValue, __COUNTER__), value, predicate)
#define TEST(testName) \
    void GCLADOS_CONCAT(__gcladosTestExecutor_, testName)(void); \
    GcladosTest GCLADOS_CONCAT(GCLADOS_TEST_PREFIX, testName)(char* description) { \
        return gcladosCreateTest(GCLADOS_CONCAT(__gcladosTestExecutor_, testName), description); \
    };                 \
    void GCLADOS_CONCAT(__gcladosTestExecutor_, testName)(void)

#endif