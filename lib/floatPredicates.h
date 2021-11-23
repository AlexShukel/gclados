#ifndef GCLADOS_FLOAT_PREDICATES_H
#define GCLADOS_FLOAT_PREDICATES_H

#include "predicate.h"

#define FLOAT_PREDICATE_DEFINITIONS(name, type)                                                                        \
    GcladosPredicate gcladosToEqual##name(type value, type precision);                                                 \
    GcladosPredicate gcladosToBeLessThan##name(type value);


FLOAT_PREDICATE_DEFINITIONS(Float, float)
FLOAT_PREDICATE_DEFINITIONS(Double, double)
FLOAT_PREDICATE_DEFINITIONS(LongDouble, long double)

#undef FLOAT_PREDICATE_DEFINITIONS

#endif