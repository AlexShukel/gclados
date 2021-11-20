#include "predicateNamespace.h"

#include "bytePredicates.h"
#include "intPredicates.h"

GcladosPredicateNamespace gclados = {
        .toEqualInt = gcladosToEqualInt,
        .toEqualBytes = gcladosToEqualBytes,
};
