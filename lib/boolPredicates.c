#include "boolPredicates.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool gcladosBoolPredicate(const bool *value, const bool *options) {
    return *options ? *value : !(*value);
}

char *gcladosBoolToString(bool value) {
    char *message = calloc(6, sizeof(char));

    strcpy(message, value ? "true" : "false");

    return message;
}

char *gcladosBoolExpectedValueToString(const bool *value, const bool *options, const bool pass) {
    return gcladosBoolToString(pass == !(*options));
}

char *gcladosBoolReceivedValueToString(const bool *value, const bool *options, const bool pass) {
    return gcladosBoolToString(*value);
}

GcladosPredicate gcladosToBeTruthy() {
    bool *options = malloc(sizeof(bool));
    *options = true;

    GcladosPredicate predicate = {
            .options = options,
            .customOutput = false,
            .usage = "gclados.toBeTruthy()",
            .receivedValueToString = (GcladosValueToStringConverter) gcladosBoolReceivedValueToString,
            .expectedValueToString = (GcladosValueToStringConverter) gcladosBoolExpectedValueToString,
            .execute = (bool(*)(void *, void *)) gcladosBoolPredicate,
            .free = NULL,
    };

    return predicate;
}

GcladosPredicate gcladosToBeFalsy() {
    bool *options = malloc(sizeof(bool));
    *options = false;

    GcladosPredicate predicate = {
            .options = options,
            .customOutput = false,
            .usage = "gclados.toBeFalsy()",
            .receivedValueToString = (GcladosValueToStringConverter) gcladosBoolReceivedValueToString,
            .expectedValueToString = (GcladosValueToStringConverter) gcladosBoolExpectedValueToString,
            .execute = (bool(*)(void *, void *)) gcladosBoolPredicate,
    };

    return predicate;
}
