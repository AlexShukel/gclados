// Author: Artiom Tretjakovas
// Description: This file contains implementations of functions which are necessary for handling statements. Function
//              prototypes are described in "statement.h" file.

#include "statement.h"

#include "ioutils.h"

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

    if(!pass) {
        gcladosAddStatementResult((GcladosStatementResult){
                .failMessage = gcladosGetFailedStatementMessage(pass, predicate, value),
                .filePath = filePath,
                .line = line,
        });
    }

    gcladosFreePredicate(&predicate);
}