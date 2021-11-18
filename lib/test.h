#ifndef GCLADOS_TEST_H
#define GCLADOS_TEST_H

#include <stdbool.h>

#include "dynamicArray.h"

typedef struct {
    bool pass;
    char* failMessage;
    int line;
    char* filePath;
} GcladosStatementResult;

typedef struct {
    void (*execute)(void);
    const char* description;
    GcladosDynamicArray statementResults;
    bool pass;
} GcladosTest;

GcladosTest gcladosCreateTest(void (*execute)(void), const char* description);

void gcladosAddStatementResult(GcladosStatementResult result);
void gcladosRunTest(GcladosTest *test);
void gcladosPrintTest(GcladosTest test);
void gcladosFreeStatementResult(GcladosStatementResult *result);
void gcladosFreeTest(GcladosTest *test);

#endif