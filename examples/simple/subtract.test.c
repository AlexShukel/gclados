#include <limits.h>

#include "gclados.h"
#include "mathutils.h"

// Should subtract 2 values
TEST(subtractTest) {
    ensure(subtract(3, 1), gclados.toEqualInt(2));
}

// Should subtract 2 values (INT_MIN)
TEST(subtractTest2) {
    ensure(subtract(INT_MIN, 1), gclados.toBeLessThanInt(INT_MIN));
}
