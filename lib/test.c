// Author: Artiom Tretjakovas
// Description: This file contains implementation of functions which are required for manipulating tests. Function
//              prototypes are described in "test.h" file.

#include "test.h"

#include <stdio.h>

#include "colors.h"
#include "ioutils.h"
#include "mutils.h"

GcladosTest gcladosCreateTest(void (*executor)(), const char *description) {
    GcladosTest test = {
            .execute = executor,
            .description = description,
    };

    return test;
}

GcladosDynamicArray *currentResults;

void gcladosAddStatementResult(GcladosStatementResult *result) {
    gcladosPush(currentResults, result);
}

GcladosTestResult gcladosRunTest(GcladosTest test) {
    GcladosTestResult testResult = {
            .pass = true,
            .statementResults = gcladosCreateDynamicArray(),
    };

    currentResults = testResult.statementResults;
    test.execute();
    currentResults = NULL;

    testResult.pass = true;

    for(size_t i = 0; i < testResult.statementResults->length; ++i) {
        GcladosStatementResult *result = gcladosGet(testResult.statementResults, i);

        if(result->pass == false) {
            testResult.pass = false;

            break;
        }
    }

    return testResult;
}

void gcladosPrintTest(GcladosTest test, GcladosTestResult testResult) {
    char *status =
            testResult.pass
                    ? gcladosColors.applyFlags("v",
                                               gcladosColors.createFlags(2,
                                                                         gcladosColors.foregroundColor(GCLADOS_GREEN),
                                                                         gcladosColors.bold()))
                    : gcladosColors.applyFlags("x",
                                               gcladosColors.createFlags(2,
                                                                         gcladosColors.foregroundColor(GCLADOS_RED),
                                                                         gcladosColors.bold()));
    printf("  %s  %s\n", status, test.description);

    free(status);

    if(!testResult.pass) {
        for(size_t i = 0; i < testResult.statementResults->length; ++i) {
            GcladosStatementResult *result = gcladosGet(testResult.statementResults, i);

            if(!result->pass) {
                printf("\n%s\n", result->failMessage);

                FILE *testFile = fopen(result->filePath, "r");

                gcladosPrintFileLines(testFile,
                                      gcladosMax(result->line - 2, 1),
                                      gcladosMax(result->line + 2, 1),
                                      result->line);
                fclose(testFile);
                char *coloredFile = gcladosColors.applyFlags(
                        result->filePath,
                        gcladosColors.createFlags(1, gcladosColors.foregroundColor(GCLADOS_CYAN)));
                printf("    in file %s\n\n", coloredFile);
                free(coloredFile);
            }
        }
    }
}

void gcladosFreeStatementResult(GcladosStatementResult *result) {
    if(result != NULL) {
        free(result->failMessage);
        free(result);
    }
}

void gcladosFreeTestResult(GcladosTestResult testResult) {
    for(size_t i = 0; i < testResult.statementResults->length; ++i) {
        gcladosFreeStatementResult(gcladosGet(testResult.statementResults, i));
    }

    gcladosFreeDynamicArray(testResult.statementResults);
}
