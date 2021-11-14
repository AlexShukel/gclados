#ifndef __PTF_TEST_H__
#define __PTF_TEST_H__

#include <stdbool.h>

#include "dynamicArray.h"

struct PtfStatementResult {
    bool pass;
    char* failMessage;
    int line;
    char* filePath;
};

struct PtfTest {
    void (*execute)(void);
    const char* description;
    struct PtfDynamicArray statementResults;
    bool pass;
};

struct PtfTest createPtfTest(void (*execute)(void), const char* description);

void ptfAddStatementResult(struct PtfStatementResult result);
void ptfRunTest(struct PtfTest *test);
void ptfPrintTest(struct PtfTest test);
void ptfFreeStatementResult(struct PtfStatementResult *result);
void ptfFreeTest(struct PtfTest *test);

#endif