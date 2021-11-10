#ifndef __PTF_TEST_SUITE_H__
#define __PTF_TEST_SUITE_H__

#include "stdlib.h"

struct PtfTestSuite {
    char* testSuiteName;
    struct PtfTest* tests;
    size_t testCount;
};

struct PtfTestSuite createPtfTestSuite(char* testSuiteName, struct PtfTest* tests, size_t testCount);

#endif
