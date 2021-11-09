#include "ptf.h"

#include <stdio.h>
#include <stdlib.h>

void createPtfStatement(int line, void* value, struct PtfPredicate predicate) {
    struct PtfPredicateResult result = predicate.execute(value, predicate.options);

    if(!result.pass) {
        printf("Test failed on line %d:\n\n%s\n", line, result.failMessage);
    }

    free(result.failMessage);
    free(predicate.options);
}
