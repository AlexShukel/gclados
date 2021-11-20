#include "sum.h"
#include "gclados.h"

// Should be equal to int
TEST(test0) {
    ensure((float) sum(1, 2), gclados.toEqualFloat(10));
}

TEST(test1) {
    ensure((float) sum(1, 2), gclados.toBeGreaterThanFloat(5));
}

TEST(test2) {
    ensure((float) sum(1, 2), gclados.toBeLessThanFloat(3));
}

TEST(test3) {
    ensure((float) sum(1, 2), gclados.toBeGreaterThanOrEqualFloat(4));
}

TEST(test4) {
    ensure((float) sum(1, 2), gclados.toBeLessThanOrEqualFloat(2));
}