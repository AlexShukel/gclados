#ifndef __PTF_TEST_H__
#define __PTF_TEST_H__

#include <stdbool.h>

#include "dynamicArray.h"

struct PtfStatementResult {
    bool pass;
    char* failMessage;
};

struct PtfTest {
    void (*execute)(void);
    const char* description;
};

struct PtfTest createPtfTest(void (*execute)(void), const char* description);

void ptfAddStatementResult(struct PtfStatementResult result);
void ptfRunTest(struct PtfTest test, struct PtfDynamicArray* result);

#endif