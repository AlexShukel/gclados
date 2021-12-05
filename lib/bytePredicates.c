// Author: Artiom Tretjakovas
// Description: This file contains implementations of byte predicates. The function prototypes are described in
//              "bytePredicates.h" file.

#include "bytePredicates.h"

#include <stdlib.h>
#include <string.h>

#include "ioutils.h"

// Structure, that saves toEqualBytes predicate options.
typedef struct GcladosEqualBytePredicateOptions {
    void *bytes;
    size_t count;
} GcladosEqualBytePredicateOptions;

// Function for converting expected value to hex string.
char *gcladosToEqualBytesExpectedValue(void *value, const GcladosEqualBytePredicateOptions *options, bool pass) {
    char *valueAsHex = gcladosConvertToHex(options->bytes, options->count);

    if(pass) {
        char *valueWithPrefix = calloc(sizeof(char), strlen(valueAsHex) + 4);
        sprintf(valueWithPrefix, "not %s", valueAsHex);
        free(valueAsHex);

        return valueWithPrefix;
    } else {
        return valueAsHex;
    }
}

// Function for converting received value to hex string.
char *gcladosToEqualBytesReceivedValue(void *value, const GcladosEqualBytePredicateOptions *options, bool pass) {
    if(pass) {
        return NULL;
    } else {
        return gcladosConvertToHex(value, options->count);
    }
}

// Function for comparing two values from pointers by each byte.
bool gcladosToEqualBytesPredicate(void *value, const GcladosEqualBytePredicateOptions *options) {
    unsigned char *realValue = (unsigned char *) value;
    unsigned char *expectedValue = (unsigned char *) options->bytes;

    for(int i = 0; i < options->count; i++) {
        if(realValue[i] != expectedValue[i]) {
            return false;
        }
    }

    return true;
}

GcladosPredicate gcladosToEqualBytes(void *bytes, size_t count) {
    GcladosEqualBytePredicateOptions *options = malloc(sizeof(GcladosEqualBytePredicateOptions));

    options->bytes = bytes;
    options->count = count;

    GcladosPredicate predicate = {
            .options = options,
            .usage = "ptf.toEqualBytes(%s, someSize)",
            .execute = (bool(*)(void *, void *)) gcladosToEqualBytesPredicate,
            .expectedValueToString = (GcladosValueToStringConverter) gcladosToEqualBytesExpectedValue,
            .receivedValueToString = (GcladosValueToStringConverter) gcladosToEqualBytesReceivedValue,
            .free = NULL,
    };

    return predicate;
}