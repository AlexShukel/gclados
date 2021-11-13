#include "statement.h"
#include "stdio.h"
#include "stdlib.h"

void createPtfStatement(int line, char* filePath, void* value, struct PtfPredicate predicate) {
    bool pass = predicate.execute(value, predicate.options);

    struct PtfStatementResult result = {
            .pass = pass,
            .failMessage = NULL,
    };

    if(!pass) {
        /*
        // TODO: make this work
        FILE* testFile = fopen(filePath, "r");

        int currentLine = 0;
        int c;
        while((c = fgetc(testFile)) != EOF && currentLine + 1 != line) {
             if(c == '\n') {
                 ++currentLine;
             }
        }

        if(c != EOF) {
            bool isFirst = false;

            printf("%3d | ", line);
            while(c = fgetc(testFile), c != EOF && c != '\n') {
                if(!isFirst && !isspace(c)) {
                    isFirst = true;
                }

                if(isFirst || !isspace(c)) {
                    printf("%c", c);
                }
            }
            printf("\n");
        }
        */
        char* message = predicate.failMessage(value, predicate.options, pass);

        result.failMessage = message;
    }

    free(predicate.options);

    ptfAddStatementResult(result);
}