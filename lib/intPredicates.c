#include "intPredicates.h"
#include "stdlib.h"
#include "stdio.h"

char* ptfIntToString(int a) {
    char* str = calloc(20, sizeof(char));

    sprintf(str, "%d", a);

    return str;
}

struct PtfPredicateResult ptfToEqualIntPredicate(void* value, const int* options) {
    int receivedValue = *((int*) value);
    bool pass = receivedValue == *options;

    char* expectedValueAsString = ptfIntToString(*options);
    char* receivedValueAsString = ptfIntToString(receivedValue);

    struct PtfPredicateResult result = {
            .failMessage = ptfStandardErrorMessage(pass, "ptf.toEqualInt(%s)", expectedValueAsString, receivedValueAsString),
            .pass = pass,
    };

    free(expectedValueAsString);
    free(receivedValueAsString);

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
