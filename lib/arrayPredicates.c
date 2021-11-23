#include "arrayPredicates.h"

#include <stdio.h>

typedef struct {
    GcladosPredicate predicate;
    size_t elementSize;
    size_t arrayLength;
} GcladosArrayPredicateOptions;

bool gcladosEachPredicate(char *value, const GcladosArrayPredicateOptions *options) {
    for(size_t i = 0; i < options->arrayLength; ++i) {
        if(!options->predicate.execute((value) + i * options->elementSize, options->predicate.options)) {
            return false;
        }
    }

    return true;
}

char *gcladosEachMessage(char *value, const GcladosArrayPredicateOptions *options, bool pass) {
    char *message = calloc(1024, sizeof(char));

    size_t offset = sprintf(message, "{ ");

    for(size_t i = 0; i < options->arrayLength; ++i) {
        char *element = options->predicate.receivedValueToString(value + i * options->elementSize,
                                                                 options->predicate.options,
                                                                 pass);

        offset += sprintf(message + offset, i == options->arrayLength - 1 ? "%s " : "%s, ", element);
        free(element);
    }

    sprintf(message + offset, "}");
    return message;
}

GcladosPredicate gcladosEach(GcladosPredicate elementPredicate, size_t elementSize, size_t arrayLength) {
    GcladosArrayPredicateOptions *options = malloc(sizeof(GcladosArrayPredicateOptions));

    options->predicate = elementPredicate;
    options->elementSize = elementSize;
    options->arrayLength = arrayLength;

    GcladosPredicate predicate = {
            .execute = (bool(*)(void *, void *)) gcladosEachPredicate,
            .usage = "gclados.each(%s, elementSize, arrayLength)",
            .expectedValueToString = (GcladosValueToStringConverter) gcladosEachMessage,
    };

    return predicate;
}
