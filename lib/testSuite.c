#include "testSuite.h"

struct PtfTestSuite createPtfTestSuite(char* testSuiteName, struct PtfTest* tests, size_t testCount) {
    struct PtfTestSuite suite = {
            .testSuiteName = testSuiteName,
            .tests = tests,
            .testCount = testCount,
    };

    return suite;
}