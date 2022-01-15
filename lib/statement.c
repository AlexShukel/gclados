// Author: Artiom Tretjakovas
// Description: This file contains implementations of functions which are necessary for handling statements. Function
//              prototypes are described in "statement.h" file.

#include "statement.h"

#include <stdlib.h>

#include "ioutils.h"
#include "test.h"

void gcladosCreateStatement(const char *functionName,
                            int line,
                            char *filePath,
                            void *value,
                            GcladosPredicate predicate) {
    StatementContext context = {
            .fileName = filePath,
            .lineNumber = line,
            .functionName = functionName,
    };

    bool pass = predicate.execute(context, value, predicate.options);

    GcladosStatementResult *result = malloc(sizeof(GcladosStatementResult));
    result->pass = pass;
    result->failMessage = NULL;
    result->line = line;
    result->filePath = filePath;

    if(!pass) {
        result->failMessage = gcladosGetFailedStatementMessage(pass, predicate, value);
    }

    gcladosFreePredicate(&predicate);

    gcladosAddStatementResult(result);
}