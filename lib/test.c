#include "test.h"

#include <stdio.h>

#include "colors.h"
#include "ioutils.h"

GcladosTest gcladosCreateTest(void (*executor)(), const char* description) {
    GcladosTest test = {
            .execute = executor,
            .description = description,
            .statementResults = gcladosCreateDynamicArray(sizeof(GcladosStatementResult)),
            .pass = false,
    };

    return test;
}

GcladosDynamicArray* currentResults;

void gcladosAddStatementResult(GcladosStatementResult result) {
    gcladosPush(currentResults, &result);
}

void gcladosRunTest(GcladosTest *test) {
    currentResults = &test->statementResults;
    test->execute();
    test->pass = true;

    for(size_t i = 0; i < test->statementResults.length; ++i) {
        GcladosStatementResult *result = gcladosGet(&test->statementResults, i);

        if(result->pass == false) {
            test->pass = false;

            break;
        }
    }
}

void gcladosPrintTest(GcladosTest test) {
    char* status = test.pass ?
            gcladosColors.applyFlags("v",
                                     gcladosColors.createFlags(2,
                                                               gcladosColors.foregroundColor(GCLADOS_GREEN),
                                                               gcladosColors.bold())) :
            gcladosColors.applyFlags("x",
                                     gcladosColors.createFlags(2,
                                                               gcladosColors.foregroundColor(GCLADOS_RED),
                                                               gcladosColors.bold()));
    printf("  %s  %s\n", status, test.description);

    free(status);

    if(!test.pass) {
        for(size_t i = 0; i < test.statementResults.length; ++i) {
            GcladosStatementResult *result = gcladosGet(&test.statementResults, i);

            if(!result->pass) {
                printf("\n%s\n", result->failMessage);

                FILE* testFile = fopen(result->filePath, "r");

                gcladosPrintFileLines(testFile, result->line - 2, result->line + 2, result->line);
                fclose(testFile);
                char* coloredFile = gcladosColors.applyFlags(result->filePath, gcladosColors.createFlags(1, gcladosColors.foregroundColor(GCLADOS_CYAN)));
                printf("    in file %s\n\n", coloredFile);
                free(coloredFile);
            }
        }
    }
}

void gcladosFreeStatementResult(GcladosStatementResult *result) {
    free(result->failMessage);
    free(result->filePath);
    free(result);
}

void gcladosFreeTest(GcladosTest *test) {
    for(size_t i = 0; i < test->statementResults.length; ++i) {
        gcladosFreeStatementResult(gcladosGet(&test->statementResults, i));
    }

    gcladosFreeDynamicArray(&test->statementResults);
}
