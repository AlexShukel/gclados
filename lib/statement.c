#include "statement.h"
#include "stdio.h"
#include "stdlib.h"

void createPtfStatement(int line, char* filePath, void* value, struct PtfPredicate predicate) {
    bool pass = predicate.execute(value, predicate.options);

    struct PtfStatementResult result = {
            .pass = pass,
            .failMessage = NULL,
            .line = line,
            .filePath = filePath,
    };

    if(!pass) {
        char* message = predicate.failMessage(value, predicate.options, pass);

        result.failMessage = message;
    }

    free(predicate.options);

    ptfAddStatementResult(result);
}