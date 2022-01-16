#include "gclados.h"

#include <string.h>

void testBoolPredicateLogic() {
    GcladosPredicate boolPredicate = gclados.toBeTruthy();
    bool value = false;
    assert(boolPredicate.execute((StatementContext){}, &value, boolPredicate.options) == false);
    value = true;
    assert(boolPredicate.execute((StatementContext){}, &value, boolPredicate.options) == true);
    value = 100;
    assert(boolPredicate.execute((StatementContext){}, &value, boolPredicate.options) == true);
    gcladosFreePredicate(&boolPredicate);

    boolPredicate = gclados.toBeFalsy();
    value = false;
    assert(boolPredicate.execute((StatementContext){}, &value, boolPredicate.options) == true);
    value = true;
    assert(boolPredicate.execute((StatementContext){}, &value, boolPredicate.options) == false);
    value = 100;
    assert(boolPredicate.execute((StatementContext){}, &value, boolPredicate.options) == false);
    gcladosFreePredicate(&boolPredicate);
}

void testBoolPredicateValueConverters() {
    GcladosPredicate boolPredicate = gclados.toBeTruthy();
    bool value = false;
    char *message = boolPredicate.receivedValueToString(&value, boolPredicate.options, false);
    assert(strcmp(message, "false") == 0);
    free(message);

    value = false;
    message = boolPredicate.receivedValueToString(&value, boolPredicate.options, true);
    assert(strcmp(message, "false") == 0);
    free(message);

    value = true;
    message = boolPredicate.receivedValueToString(&value, boolPredicate.options, false);
    assert(strcmp(message, "true") == 0);
    free(message);

    value = true;
    message = boolPredicate.receivedValueToString(&value, boolPredicate.options, true);
    assert(strcmp(message, "true") == 0);
    free(message);

    message = boolPredicate.expectedValueToString(&value, boolPredicate.options, false);
    assert(strcmp(message, "true") == 0);
    free(message);

    // Should return inverted value.
    message = boolPredicate.expectedValueToString(&value, boolPredicate.options, true);
    assert(strcmp(message, "false") == 0);
    free(message);

    gcladosFreePredicate(&boolPredicate);

    boolPredicate = gclados.toBeFalsy();
    message = boolPredicate.expectedValueToString(&value, boolPredicate.options, false);
    assert(strcmp(message, "false") == 0);
    free(message);

    // Should return inverted value.
    message = boolPredicate.expectedValueToString(&value, boolPredicate.options, true);
    assert(strcmp(message, "true") == 0);
    free(message);
}

void runBoolPredicateTests() {
    testBoolPredicateLogic();
    testBoolPredicateValueConverters();
}