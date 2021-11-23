#include "sum.h"
#include "gclados.h"
#include <float.h>

// Should be equal to int
TEST(test0) {
    ensure(1 + 2, gclados.not(gclados.toBeLessThanOrEqualInt(4)));
}