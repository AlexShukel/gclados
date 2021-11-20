#ifndef GCLADOS_TEST_H
#define GCLADOS_TEST_H

#include <stdbool.h>

#include "dynamicArray.h"

typedef struct {
    bool pass;
    char *failMessage;
    int line;
    char *filePath;
} GcladosStatementResult;

typedef struct {
    void (*execute)(void);
    const char *description;
} GcladosTest;

typedef struct {
    bool pass;
    GcladosDynamicArray *statementResults;
} GcladosTestResult;

GcladosTest gcladosCreateTest(void (*execute)(void), const char *description);

void gcladosAddStatementResult(GcladosStatementResult *result);
GcladosTestResult gcladosRunTest(GcladosTest test);
void gcladosPrintTest(GcladosTest test, GcladosTestResult result);
void gcladosFreeTestResult(GcladosTestResult testResult);

#endif