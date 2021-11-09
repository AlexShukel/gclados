#include "predicateNamespace.h"

struct PtfPredicateNamespace ptf = {
        .toEqualInt = ptfToEqualInt,
        .toEqualBytes = ptfToEqualBytes,
};
