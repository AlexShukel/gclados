#ifndef GCLADOS_PREDICATE_NAMESPACE_H
#define GCLADOS_PREDICATE_NAMESPACE_H

#include <stdlib.h>

#include "predicate.h"

#define NUMBER_PREDICATE_DEFINITIONS(name, type)                                                                       \
    GcladosPredicate (*toEqual##name)(type value);                                                                     \
    GcladosPredicate (*toBeLessThan##name)(type value);                                                                \
    GcladosPredicate (*toBeGreaterThan##name)(type value);                                                             \
    GcladosPredicate (*toBeLessThanOrEqual##name)(type value);                                                         \
    GcladosPredicate (*toBeGreaterThanOrEqual##name)(type value);

#define FLOAT_PREDICATE_DEFINITIONS(name, type)                                                                        \
    GcladosPredicate (*toEqual##name)(type value, type precision);                                                     \
    GcladosPredicate (*toBeLessThan##name)(type value);                                                                \
    GcladosPredicate (*toBeGreaterThan##name)(type value);

typedef struct {
    // Number predicates
    NUMBER_PREDICATE_DEFINITIONS(Char, signed char)
    NUMBER_PREDICATE_DEFINITIONS(UnsignedChar, unsigned char)
    NUMBER_PREDICATE_DEFINITIONS(Short, signed short)
    NUMBER_PREDICATE_DEFINITIONS(UnsignedShort, unsigned short)
    NUMBER_PREDICATE_DEFINITIONS(Int, signed int)
    NUMBER_PREDICATE_DEFINITIONS(Unsigned, unsigned int)
    NUMBER_PREDICATE_DEFINITIONS(Long, signed long)
    NUMBER_PREDICATE_DEFINITIONS(UnsignedLong, unsigned long)

    // Float predicates
    FLOAT_PREDICATE_DEFINITIONS(Float, float)
    FLOAT_PREDICATE_DEFINITIONS(Double, double)
    FLOAT_PREDICATE_DEFINITIONS(LongDouble, long double)

    // String predicates
    GcladosPredicate (*toEqualString)(char *string);

    // Boolean predicates
    GcladosPredicate (*toBeTruthy)();
    GcladosPredicate (*toBeFalsy)();

    // Byte / pointer predicates
    GcladosPredicate (*toEqualBytes)(void *bytes, size_t size);

    // Array predicates
    GcladosPredicate (*each)(GcladosPredicate predicate, size_t elementSize, size_t arrayLength);

    // Miscellaneous
    GcladosPredicate (*not )(GcladosPredicate predicate);
} GcladosPredicateNamespace;

#undef NUMBER_PREDICATE_DEFINITIONS
#undef FLOAT_PREDICATE_DEFINITIONS

extern GcladosPredicateNamespace gclados;

#endif
