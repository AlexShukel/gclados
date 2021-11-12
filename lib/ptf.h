#ifndef __PTF_LIB_H__
#define __PTF_LIB_H__

#include "predicate.h"
#include "predicateNamespace.h"
#include "test.h"
#include "testSuite.h"
#include "colors.h"

void createPtfStatement(int line, char* filename, void* value, struct PtfPredicate predicate);
void runPtfTestSuites(struct PtfTestSuite* suites, size_t count);

#define PTF_CONCAT(a, b) PTF_CONCAT_INNER(a, b)
#define PTF_CONCAT_INNER(a, b) a ## b
#define PTF_ENSURE_INNER(varName, value, predicate) typeof(value) (varName) = value; createPtfStatement(__LINE__, __FILE__, &(varName), predicate)

#define PTF_TEST_PREFIX __ptfTest_
#define ensure(value, predicate) PTF_ENSURE_INNER(PTF_CONCAT(__ptfValue, __COUNTER__), value, predicate)
#define TEST(testName) \
    void PTF_CONCAT(__ptfTestExecutor_, testName)(void); \
    struct PtfTest PTF_CONCAT(PTF_TEST_PREFIX, testName)(char* description) { \
        return createPtfTest(PTF_CONCAT(__ptfTestExecutor_, testName), description); \
    };                 \
    void PTF_CONCAT(__ptfTestExecutor_, testName)(void)

#endif