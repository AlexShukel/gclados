#ifndef GCLADOS_NUMBER_PREDICATES_H
#define GCLADOS_NUMBER_PREDICATES_H

#include "predicate.h"

#define NUMBER_PREDICATE_DEFINITIONS(name, type)                                                                       \
    GcladosPredicate gcladosToEqual##name(type value);                                                                 \
    GcladosPredicate gcladosToBeLessThan##name(type value);                                                            \
    GcladosPredicate gcladosToBeGreaterThan##name(type value);                                                         \
    GcladosPredicate gcladosToBeLessThanOrEqual##name(type value);                                                     \
    GcladosPredicate gcladosToBeGreaterThanOrEqual##name(type value);

NUMBER_PREDICATE_DEFINITIONS(Char, signed char)
NUMBER_PREDICATE_DEFINITIONS(UnsignedChar, unsigned char)
NUMBER_PREDICATE_DEFINITIONS(Short, signed short)
NUMBER_PREDICATE_DEFINITIONS(UnsignedShort, unsigned short)
NUMBER_PREDICATE_DEFINITIONS(Int, signed int)
NUMBER_PREDICATE_DEFINITIONS(Unsigned, unsigned int)
NUMBER_PREDICATE_DEFINITIONS(Long, signed long)
NUMBER_PREDICATE_DEFINITIONS(UnsignedLong, unsigned long)
NUMBER_PREDICATE_DEFINITIONS(Float, float)
NUMBER_PREDICATE_DEFINITIONS(Double, double)
NUMBER_PREDICATE_DEFINITIONS(LongDouble, long double)

#undef NUMBER_PREDICATE_DEFINITIONS

#endif
