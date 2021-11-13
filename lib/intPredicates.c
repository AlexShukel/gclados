#include "intPredicates.h"
#include "stdlib.h"
#include "stdio.h"

char *ptfIntToString(int a) {
    char *str = calloc(20, sizeof(char));

    sprintf(str, "%d", a);

    return str;
}

char *ptfToEqualIntMessage(const int *options, void *value, bool pass) {
    char *expectedValueAsString = ptfIntToString(*options);
    char *receivedValueAsString = ptfIntToString(*((int*) value));

    char *message = ptfStandardErrorMessage(pass, "ptf.toEqualInt(%s)", expectedValueAsString, receivedValueAsString);

    free(expectedValueAsString);
    free(receivedValueAsString);

    return message;
}

bool ptfToEqualIntPredicate(void* value, const int* options) {
    return *((int*) value) == *options;
};

struct PtfPredicate ptfToEqualInt(int value) {
    int *options = malloc(sizeof(int));
    *options = value;
    struct PtfPredicate predicate = {
            .options = options,
            .execute = (bool (*)(void*, void*)) ptfToEqualIntPredicate,
            .failMessage = (char *(*)(void *, void *, bool)) ptfToEqualIntMessage,
    };

    return predicate;
}
