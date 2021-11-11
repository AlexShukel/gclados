#include "test.h"

struct PtfTest createPtfTest(void (*executor)(), const char* description) {
    struct PtfTest test = {
            .execute = executor,
            .description = description
    };

    return test;
}
