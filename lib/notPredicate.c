// Author: Artiom Tretjakovas
// Description: This file contains implementation of not predicate. Function prototype is in "notPredicate.h" file.

#include "notPredicate.h"

#include <stdio.h>
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

void gcladosFreeNotPredicate(GcladosPredicate *predicate) {
    gcladosFreePredicate(predicate->options);

    free(predicate->usage);
}

GcladosPredicate gcladosNot(GcladosPredicate optionsPredicate) {
    GcladosPredicate *options = malloc(sizeof(GcladosPredicate));

    *options = optionsPredicate;

    char *usage = calloc(1024, sizeof(char));
    sprintf(usage, "gclados.not(%s)", optionsPredicate.usage);

    GcladosPredicate predicate = {
            .options = options,
            .usage = usage,
            .customOutput = optionsPredicate.customOutput,
            .receivedValueToString = NULL,
            .expectedValueToString = (GcladosValueToStringConverter) gcladosNotExpectedValue,
            .execute = (bool(*)(void *, void *)) gcladosNotPredicate,
            .free = gcladosFreeNotPredicate,
    };

    if(optionsPredicate.receivedValueToString != NULL) {
        predicate.receivedValueToString = (GcladosValueToStringConverter) gcladosNotReceivedValue;
    }

    return predicate;
}
