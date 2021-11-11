#include "ptf.h"

#include <stdio.h>
#include <stdlib.h>

void runPtfTestSuites(struct PtfTestSuite* suites, size_t count) {

    for(int i = 0; i < count; i++) {

    }

}

void ptfDrawTests() {
    // Clear screen
    printf("\x1b[H\x1b[2J");

}

void createPtfStatement(int line, char* filePath, void* value, struct PtfPredicate predicate) {
    struct PtfPredicateResult result = predicate.execute(value, predicate.options);

    if(!result.pass) {
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
        printf("Test failed on line %d:\n\n%s\n", line, result.failMessage);
    }

    free(result.failMessage);
    free(predicate.options);
}
