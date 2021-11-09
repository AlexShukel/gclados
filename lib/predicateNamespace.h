#ifndef __PTF_PREDICATE_NAMESPACE_H__
#define __PTF_PREDICATE_NAMESPACE_H__

#include "predicate.h"
#include "intPredicates.h"
#include "bytePredicates.h"

struct PtfPredicateNamespace {
    struct PtfPredicate (*toEqualInt)(int value);
    struct PtfPredicate (*toEqualBytes)(void* bytes, size_t size);
};

extern struct PtfPredicateNamespace ptf;

#endif
