#include "sum.h"
#include "gclados.h"
#include <float.h>
#include <string.h>

TEST(test1) {
    int a[] = {1, 2};
    ensure("Hello world!", gclados.not(gclados.toEqualString("Hello world!")));
}