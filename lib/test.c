// Author: Artiom Tretjakovas
// Description: This file contains implementation of functions which are required for manipulating tests. Function
//              prototypes are described in "test.h" file.

#include "test.h"

#include <setjmp.h>
#include <stdio.h>

#include "colors.h"
#include "ioutils.h"
#include "mutils.h"
#include "snapshots.h"

const int GCLADOS_FAILED_TEST_RESULT = 1;

GcladosTest gcladosCreateTest(void (*executor)(), const char *description) {
    GcladosTest test = {
            .execute = executor,
            .description = description,
    };

    return test;
}

GcladosStatementResult gcladosStatementResult;
jmp_buf gcladosJmpBuff;

void gcladosAddStatementResult(GcladosStatementResult result) {
    gcladosStatementResult = result;
    longjmp(gcladosJmpBuff, GCLADOS_FAILED_TEST_RESULT);
}

GcladosTestResult gcladosRunTest(GcladosTest test) {
    int result = setjmp(gcladosJmpBuff);

    GcladosTestResult testResult = {};

    if(result == 0) {
        gcladosResetSnapshotCounter();
        test.execute();
        testResult.pass = true;
    } else {
        testResult.pass = false;
        testResult.statementResult = gcladosStatementResult;
    }

    return testResult;
}

void gcladosPrintTest(GcladosTest test, GcladosTestResult testResult) {
    GcladosAnsiFlags failureStyle = gcladosDefaultFailureStyle();
    GcladosAnsiFlags successStyle = gcladosDefaultSuccessStyle();
    char *status =
            testResult.pass ? gcladosColors.applyFlags("v", successStyle) : gcladosColors.applyFlags("x", failureStyle);
    printf("  %s  %s\n", status, test.description);

    free(status);

    if(!testResult.pass) {
        printf("\n%s\n", testResult.statementResult.failMessage);

        FILE *testFile = fopen(testResult.statementResult.filePath, "r");

        gcladosPrintFileLines(testFile,
                              gcladosMax(testResult.statementResult.line - 2, 1),
                              gcladosMax(testResult.statementResult.line + 2, 1),
                              testResult.statementResult.line);
        fclose(testFile);

        GcladosAnsiFlags filePathStyle = gcladosColors.createFlags(1, gcladosColors.foregroundColor(GCLADOS_CYAN));

        char *coloredFile = gcladosColors.applyFlags(testResult.statementResult.filePath, filePathStyle);
        printf("    in file %s\n\n", coloredFile);

        gcladosColors.freeFlags(filePathStyle);
        free(coloredFile);
    }

    gcladosColors.freeFlags(successStyle);
    gcladosColors.freeFlags(failureStyle);
}

void gcladosFreeStatementResult(GcladosStatementResult *result) {
    if(result != NULL) {
        free(result->failMessage);
        free(result);
    }
}
