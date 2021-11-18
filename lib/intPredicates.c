#include "intPredicates.h"

#include <stdlib.h>
#include <stdio.h>

#include "colors.h"
#include "ioutils.h"

char *gcladosIntToString(int a) {
    char *str = calloc(20, sizeof(char));

    sprintf(str, "%d", a);

    return str;
}

char *gcladosToEqualIntMessage(const int *options, void *value, bool pass) {
    char *expectedValueAsString = gcladosIntToString(*options);
    char *receivedValueAsString = gcladosIntToString(*((int*) value));

    char *message = gcladosStandardErrorMessage(pass, "gclados.toEqualInt(%s)", expectedValueAsString, receivedValueAsString);

    free(expectedValueAsString);
    free(receivedValueAsString);

    return message;
}

bool gcladosToEqualIntPredicate(void* value, const int* options) {
    return *((int*) value) == *options;
};

GcladosPredicate gcladosToEqualInt(int value) {
    int *options = malloc(sizeof(int));
    *options = value;
    GcladosPredicate predicate = {
            .options = options,
            .execute = (bool (*)(void*, void*)) gcladosToEqualIntPredicate,
            .failMessage = (char *(*)(void *, void *, bool)) gcladosToEqualIntMessage,
    };

    return predicate;
}
