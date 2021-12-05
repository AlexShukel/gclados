#include "gclados.h"

// Not predicate
TEST(test0) {
    ensure(1, gclados.not(gclados.toBeLessThanInt(1)));
}

// Array predicate
TEST(test1) {
    int a[] = {1, 2, 3, 4, 5};
    ensureArray(a, gclados.each(gclados.toBeGreaterThanInt(1), sizeof(int), 5));
}
