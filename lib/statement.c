#include "statement.h"

#include <stdlib.h>

#include "test.h"

void gcladosCreateStatement(int line, char* filePath, void* value, GcladosPredicate predicate) {
    bool pass = predicate.execute(value, predicate.options);

    GcladosStatementResult result = {
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

    gcladosAddStatementResult(result);
}