#include "sum.h"
#include "gclados.h"

int sum(int a, int b) {
    ensure(3, gclados.toEqualInt(2));
    return a + b;
}