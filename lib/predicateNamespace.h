#ifndef __PTF_PREDICATE_NAMESPACE_H__
#define __PTF_PREDICATE_NAMESPACE_H__

#include "predicate.h"
#include "intPredicates.h"

struct PtfPredicateNamespace {
    struct PtfPredicate (*toEqualInt)(int value);
};

extern struct PtfPredicateNamespace ptf;

#endif
