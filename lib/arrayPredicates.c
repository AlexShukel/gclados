// Author: Artiom Tretjakovas
// Description: This file contains the implementations of the array predicates. Function prototypes is in the
// "arrayPredicates.h" file.

#include "arrayPredicates.h"

#include <stdio.h>

// Structure for saving array predicate options.
typedef struct {
    GcladosPredicate predicate;
    size_t elementSize;
    size_t arrayLength;
} GcladosArrayPredicateOptions;

// "each" predicate logic.
bool gcladosEachPredicate(StatementContext context, char **value, const GcladosArrayPredicateOptions *options) {
    for(size_t i = 0; i < options->arrayLength; ++i) {
        if(!options->predicate.execute(context, (*value) + i * options->elementSize, options->predicate.options)) {
            return false;
        }
    }

    return true;
}

// TODO: make this message beautiful
// Received / expected value converter to string for the "each" predicate.
char *gcladosEachMessage(char **value, const GcladosArrayPredicateOptions *options, bool pass) {
    char *message = calloc(1024, sizeof(char));

    size_t offset = sprintf(message, "{ ");

    for(size_t i = 0; i < options->arrayLength; ++i) {
        void *elementValue = *value + i * options->elementSize;
        char *element = options->predicate.receivedValueToString(elementValue, options->predicate.options, pass);

        char *format;
        bool elementPassed = !(options->predicate.execute((StatementContext){}, elementValue, options->predicate.options)) == pass;

        if(i == options->arrayLength - 1) {
            if(elementPassed) {
                format = "%s ";
            } else {
                format = "(%s) ";
            }
        } else {
            if(elementPassed) {
                format = "%s, ";
            } else {
                format = "(%s), ";
            }
        }

        offset += sprintf(message + offset, format, element);
        free(element);
    }

    sprintf(message + offset, "}");
    return message;
}

// Cleanup for the "each" predicate.
void gcladosFreeEachPredicate(GcladosPredicate *predicate) {
    GcladosArrayPredicateOptions *options = predicate->options;

    gcladosFreePredicate(&(options->predicate));
}

// "each" predicate constructor.
GcladosPredicate gcladosEach(GcladosPredicate elementPredicate, size_t elementSize, size_t arrayLength) {
    GcladosArrayPredicateOptions *options = malloc(sizeof(GcladosArrayPredicateOptions));

    options->predicate = elementPredicate;
    options->elementSize = elementSize;
    options->arrayLength = arrayLength;

    GcladosPredicate predicate = {
            .execute = (bool(*)(StatementContext, void *, void *)) gcladosEachPredicate,
            .usage = "gclados.each(%s, elementSize, arrayLength)",
            .customOutput = true,
            .expectedValueToString = (GcladosValueToStringConverter) gcladosEachMessage,
            .receivedValueToString = (GcladosValueToStringConverter) gcladosEachMessage,
            .options = options,
            .free = gcladosFreeEachPredicate,
    };

    return predicate;
}
