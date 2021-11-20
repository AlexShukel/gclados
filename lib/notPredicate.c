#include "notPredicate.h"

#include <stdlib.h>

char *gcladosNotMessage(void *value, GcladosPredicate *predicate, bool pass) {
    return predicate->failMessage(value, predicate->options, !pass);
}

bool gcladosNotPredicate(void *value, GcladosPredicate *predicate) {
    return !predicate->execute(value, predicate->options);
}

GcladosPredicate gcladosNot(GcladosPredicate optionsPredicate) {
    GcladosPredicate *options = malloc(sizeof(GcladosPredicate));

    *options = optionsPredicate;

    GcladosPredicate predicate = {
            .options = options,
            .failMessage = (char *(*) (void *, void *, bool) ) gcladosNotMessage,
            .execute = (bool(*)(void *, void *)) gcladosNotPredicate,
    };

    return predicate;
}
