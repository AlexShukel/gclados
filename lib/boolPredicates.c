// Author: Artiom Tretjakovas
// Description: This file contains implementations of the boolean predicates. Function prototypes are described in
// "boolPredicates.h"

#include "boolPredicates.h"

#include <stdlib.h>
#include <string.h>

// Boolean predicate logic.
bool gcladosBoolPredicate(StatementContext context, const bool *value, const bool *options) {
    return *options ? *value : !(*value);
}

// Boolean to string converter.
char *gcladosBoolToString(bool value) {
    char *message = calloc(6, sizeof(char));

    strcpy(message, value ? "true" : "false");

    return message;
}

// Function, that converts expected boolean value to string.
char *gcladosBoolExpectedValueToString(const bool *value, const bool *options, const bool pass) {
    return gcladosBoolToString(pass == !(*options));
}

// Function, that converts received boolean value to string.
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
            .execute = (bool(*)(StatementContext, void *, void *)) gcladosBoolPredicate,
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
            .execute = (bool(*)(StatementContext, void *, void *)) gcladosBoolPredicate,
    };

    return predicate;
}
