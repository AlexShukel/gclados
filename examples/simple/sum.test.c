#include <float.h>

#include "gclados.h"
#include "mathutils.h"

// Should sum values
TEST(test1) {
    ensure(sum(0.3f, 0.6f), gclados.toEqualFloat(0.9f, 0));
}

// Should sum values (FLT_EPSILON)
TEST(test2) {
    ensure(sum(0.3f, 0.6f), gclados.toEqualFloat(0.9f, FLT_EPSILON));
}