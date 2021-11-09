#ifndef __PTF_PREDICATE_H__
#define __PTF_PREDICATE_H__

#include <stdbool.h>

struct PtfPredicateResult {
    bool pass;
    char* failMessage;
};

struct PtfPredicate {
    struct PtfPredicateResult (*execute)(void* value, void* options);
    void* options;
};

#endif