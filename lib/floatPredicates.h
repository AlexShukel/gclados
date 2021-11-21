#ifndef GCLADOS_FLOAT_PREDICATES_H
#define GCLADOS_FLOAT_PREDICATES_H

#include "predicate.h"

#define FLOAT_PREDICATE_DEFINITIONS(name, type) GcladosPredicate gcladosToEqual##name(type value, type precision);

FLOAT_PREDICATE_DEFINITIONS(Float, float)

#undef FLOAT_PREDICATE_DEFINITIONS

#endif