#include "bytePredicates.h"

#include <stdlib.h>

#include "ioutils.h"

typedef struct GcladosEqualBytePredicateOptions {
    void *bytes;
    size_t count;
} GcladosEqualBytePredicateOptions;

char *gcladosToEqualBytesMessage(void *value, const GcladosEqualBytePredicateOptions *options, bool pass) {
    char *realValueAsHex = gcladosConvertToHex(value, options->count);
    char *expectedValueAsHex = gcladosConvertToHex(options->bytes, options->count);

    char *message =
            gcladosStandardErrorMessage(pass, "ptf.toEqualBytes(%s, someSize)", expectedValueAsHex, realValueAsHex);

    free(realValueAsHex);
    free(expectedValueAsHex);

    return message;
}

bool gcladosToEqualBytesPredicate(void *value, const GcladosEqualBytePredicateOptions *options) {
    unsigned char *realValue = (unsigned char *) value;
    unsigned char *expectedValue = (unsigned char *) options->bytes;

    bool pass = true;
    for(int i = 0; i < options->count; i++) {
        if(realValue[i] != expectedValue[i]) {
            pass = false;
            break;
        }
    }

    return pass;
}

GcladosPredicate gcladosToEqualBytes(void *bytes, size_t count) {
    GcladosEqualBytePredicateOptions *options = malloc(sizeof(GcladosEqualBytePredicateOptions));

    options->bytes = bytes;
    options->count = count;

    GcladosPredicate predicate = {
            .options = options,
            .execute = (bool(*)(void *, void *)) gcladosToEqualBytesPredicate,
            .failMessage = (char *(*) (void *, void *, bool) ) gcladosToEqualBytesMessage,
    };

    return predicate;
}