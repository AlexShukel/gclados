#ifndef GCLADOS_PREDICATE_NAMESPACE_H
#define GCLADOS_PREDICATE_NAMESPACE_H

#include <stdlib.h>

#include "predicate.h"

typedef struct {
    GcladosPredicate (*toEqualInt)(int value);
    GcladosPredicate (*toEqualBytes)(void* bytes, size_t size);
} GcladosPredicateNamespace;

extern GcladosPredicateNamespace gclados;

#endif
