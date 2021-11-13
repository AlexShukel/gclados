#include "test.h"

#include <stdio.h>

#include "colors.h"
#include "ioutils.h"

struct PtfTest createPtfTest(void (*executor)(), const char* description) {
    struct PtfTest test = {
            .execute = executor,
            .description = description,
            .statementResults = createPtfDynamicArray(sizeof(struct PtfStatementResult)),
            .pass = false,
    };

    return test;
}

struct PtfDynamicArray* currentResults;

void ptfAddStatementResult(struct PtfStatementResult result) {
    ptfPush(currentResults, &result);
}

void ptfRunTest(struct PtfTest *test) {
    currentResults = &test->statementResults;
    test->execute();
    test->pass = true;

    for(size_t i = 0; i < test->statementResults.length; ++i) {
        struct PtfStatementResult *result = ptfGet(&test->statementResults, i);

        if(result->pass == false) {
            test->pass = false;

            break;
        }
    }
}

void ptfPrintTest(struct PtfTest test) {
    char* status = test.pass ?
            ptfColors.applyFlags("v", ptfColors.createFlags(2, ptfColors.foregroundColor(PTF_GREEN), ptfColors.bold())) :
            ptfColors.applyFlags("x", ptfColors.createFlags(2, ptfColors.foregroundColor(PTF_RED), ptfColors.bold()));
    printf("  %s  %s\n", status, test.description);

    free(status);

    if(!test.pass) {
        for(size_t i = 0; i < test.statementResults.length; ++i) {
            struct PtfStatementResult *result = ptfGet(&test.statementResults, i);

            if(!result->pass) {
                printf("\n%s\n", result->failMessage);

                FILE* testFile = fopen(result->filePath, "r");

                ptfPrintFileLines(testFile, result->line - 2, result->line + 2, result->line);
                fclose(testFile);
                char* coloredFile = ptfColors.applyFlags(result->filePath, ptfColors.createFlags(1, ptfColors.foregroundColor(PTF_CYAN)));
                printf("    in file %s\n\n", coloredFile);
                free(coloredFile);
            }
        }
    }
}
