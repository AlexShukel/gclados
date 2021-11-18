#include "gclados.h"

TEST(shouldDoSomething) {
    ensure(15, gclados.toEqualInt(15));
}

TEST(sss) {
    int ptr = 15;
    ensure(15, gclados.toEqualBytes(&ptr, sizeof(int)));
}