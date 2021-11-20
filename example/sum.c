#include "sum.h"
#include "gclados.h"

int sum(int a, int b) {
    ensure(2, gclados.not(gclados.toEqualInt(2)));
    return a + b;
}