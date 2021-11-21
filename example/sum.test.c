#include "sum.h"
#include "gclados.h"
#include <float.h>

// Should be equal to int
TEST(test0) {
    ensure(0.3f + 0.6f, gclados.toEqualFloat(0.9f, FLT_EPSILON));
}