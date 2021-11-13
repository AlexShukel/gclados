#include "ptf.h"

TEST(shouldDoSomething) {
    int sum2 = 0;
    for(int i = 0; i < 1000000000; ++i) {
        sum2 += i;
    }
    ensure(15, ptf.toEqualInt(15));
}

TEST(sss) {
    int sum2 = 0;
    for(int i = 0; i < 1000000000; ++i) {
        sum2 += i;
    }
    int ptr = 15;
    ensure(15, ptf.toEqualBytes(&ptr, sizeof(int)));
}