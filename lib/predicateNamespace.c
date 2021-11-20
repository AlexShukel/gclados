#include "predicateNamespace.h"

#include "bytePredicates.h"
#include "intPredicates.h"
#include "notPredicate.h"

GcladosPredicateNamespace gclados = {
        .toEqualInt = gcladosToEqualInt,
        .toEqualBytes = gcladosToEqualBytes,
        .not = gcladosNot,
};
