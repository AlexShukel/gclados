#include "sum.h"
#include "gclados.h"
#include <float.h>

// Should be equal to int
TEST(test0) {
    int array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 20};

    gcladosCreateStatement(__LINE__, __FILE__, &(array), gclados.each(gclados.toBeLessThanInt(10), sizeof(int), 10));

    // ensure(array, gclados.each(gclados.toBeLessThanInt(10), sizeof(int), 10));
}