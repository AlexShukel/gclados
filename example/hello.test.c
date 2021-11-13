#include "ptf.h"

TEST(shouldDoSomething) {
    ensure(15, ptf.toEqualInt(15));
}

TEST(sss) {
    int ptr = 15;
    ensure(15, ptf.toEqualBytes(&ptr, sizeof(int)));
}