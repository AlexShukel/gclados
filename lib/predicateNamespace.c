#include "predicateNamespace.h"

#include "bytePredicates.h"
#include "notPredicate.h"
#include "numberPredicates.h"

#define NUMBER_PREDICATE_ASSIGNMENTS(name)                                                                             \
    .toEqual##name = gcladosToEqual##name, .toBeLessThan##name = gcladosToBeLessThan##name,                            \
    .toBeGreaterThan##name = gcladosToBeGreaterThan##name,                                                             \
    .toBeLessThanOrEqual##name = gcladosToBeLessThanOrEqual##name,                                                     \
    .toBeGreaterThanOrEqual##name = gcladosToBeGreaterThanOrEqual##name,

GcladosPredicateNamespace gclados = {
        // clang-format off
        NUMBER_PREDICATE_ASSIGNMENTS(Char)
        NUMBER_PREDICATE_ASSIGNMENTS(UnsignedChar)
        NUMBER_PREDICATE_ASSIGNMENTS(Short)
        NUMBER_PREDICATE_ASSIGNMENTS(UnsignedShort)
        NUMBER_PREDICATE_ASSIGNMENTS(Int)
        NUMBER_PREDICATE_ASSIGNMENTS(Unsigned)
        NUMBER_PREDICATE_ASSIGNMENTS(Long)
        NUMBER_PREDICATE_ASSIGNMENTS(UnsignedLong)
        NUMBER_PREDICATE_ASSIGNMENTS(Float)
        NUMBER_PREDICATE_ASSIGNMENTS(Double)
        NUMBER_PREDICATE_ASSIGNMENTS(LongDouble)
        .toEqualBytes = gcladosToEqualBytes,
        // clang-format on
        .not = gcladosNot,
};

#undef NUMBER_PREDICATE_ASSIGNMENTS
