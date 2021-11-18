#include "predicateNamespace.h"

#include "intPredicates.h"
#include "bytePredicates.h"

GcladosPredicateNamespace gclados = {
        .toEqualInt = gcladosToEqualInt,
        .toEqualBytes = gcladosToEqualBytes,
};
