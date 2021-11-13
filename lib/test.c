#include "test.h"

struct PtfTest createPtfTest(void (*executor)(), const char* description) {
    struct PtfTest test = {
            .execute = executor,
            .description = description
    };

    return test;
}

struct PtfDynamicArray* currentResults;

void ptfAddStatementResult(struct PtfStatementResult result) {
    ptfPush(currentResults, &result);
}

void ptfRunTest(struct PtfTest test, struct PtfDynamicArray* result) {
    currentResults = result;
    test.execute();
}