#include "sum.h"
#include "gclados.h"
#include <float.h>
#include <string.h>

// Should be equal to int
TEST(test0) {
    int a[3] = {1, 2, 3};
    int b[3] = {4, 10, 6};
    int c[3] = {7, 8, 9};
    int *array[3] = {a, b, c};

    ensureArray(array, gclados.each(gclados.each(gclados.toBeLessThanInt(10), sizeof(int), 3), sizeof(int *), 3));
}