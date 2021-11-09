#include "intPredicates.h"
#include "stdlib.h"
#include "stdio.h"

struct PtfPredicateResult ptfToEqualIntPredicate(void* value, const int* options) {
    char* message = calloc(80, sizeof(char));
    int expectedValue = *((int*) value);
    bool pass = expectedValue == *options;

    sprintf(message, pass ? "    Expected: not %d" : "    Expected: %d\n    Received: %d", expectedValue, *options);

    struct PtfPredicateResult result = {
            .failMessage = message,
            .pass = pass,
    };

    return result;
};

struct PtfPredicate ptfToEqualInt(int value) {
    int* options = malloc(sizeof(int));
    *options = value;
    struct PtfPredicate predicate = {
            .options = options,
            .execute = (struct PtfPredicateResult (*)(void*, void*)) ptfToEqualIntPredicate,
    };

    return predicate;
}
