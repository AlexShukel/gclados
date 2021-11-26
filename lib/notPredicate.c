#include "notPredicate.h"

#include <stdlib.h>

char *gcladosNotExpectedValue(void *value, GcladosPredicate *predicate, bool pass) {
    return predicate->expectedValueToString(value, predicate->options, !pass);
}

char *gcladosNotReceivedValue(void *value, GcladosPredicate *predicate, bool pass) {
    return predicate->receivedValueToString(value, predicate->options, !pass);
}

bool gcladosNotPredicate(void *value, GcladosPredicate *predicate) {
    return !predicate->execute(value, predicate->options);
}

GcladosPredicate gcladosNot(GcladosPredicate optionsPredicate) {
    GcladosPredicate *options = malloc(sizeof(GcladosPredicate));

    *options = optionsPredicate;

    GcladosPredicate predicate = {
            .options = options,
            .usage = optionsPredicate.usage,
            .customOutput = optionsPredicate.customOutput,
            .receivedValueToString = NULL,
            .expectedValueToString = (GcladosValueToStringConverter) gcladosNotExpectedValue,
            .execute = (bool(*)(void *, void *)) gcladosNotPredicate,
    };

    if(optionsPredicate.receivedValueToString != NULL) {
        predicate.receivedValueToString = (GcladosValueToStringConverter) gcladosNotReceivedValue;
    }

    return predicate;
}
