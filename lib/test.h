// Author: Artiom Tretjakovas
// Description: This file contains functions for executing / manipulating tests.

#ifndef GCLADOS_TEST_H
#define GCLADOS_TEST_H

#include <stdbool.h>

#include "dynamicArray.h"
#include "predicate.h"

// Function, which saves statement result.
typedef struct {
    char *failMessage;
    bool pass;
    // Line of statement.
    int line;
    // Path to the file, where statement is located.
    char *filePath;
} GcladosStatementResult;

// Function, which contains information about test.
typedef struct {
    // Function which executes test.
    void (*execute)(void);
    // Description of test.
    const char *description;
} GcladosTest;

// Function, which contains test result.
typedef struct {
    bool pass;
    GcladosDynamicArray *statementResults;
} GcladosTestResult;

// Function, which creates new test.
GcladosTest gcladosCreateTest(void (*execute)(void), const char *description);

// Function which registers new statement result.
void gcladosAddStatementResult(GcladosStatementResult *result);
// Function, which runs given test.
GcladosTestResult gcladosRunTest(GcladosTest test);
// Function, which prints given test.
void gcladosPrintTest(GcladosTest test, GcladosTestResult result);
// Function which disposes given test result.
void gcladosFreeTestResult(GcladosTestResult testResult);

#endif