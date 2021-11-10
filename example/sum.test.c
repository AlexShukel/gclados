#include "sum.h"
#include "ptf.h"

// This is some test
TEST(test0) {
    unsigned short value = 65535;

    ensure(-65535, ptf.toEqualBytes(&value, sizeof(short)));
}

// This is some test
TEST(test1) {
    unsigned short value = 65535;

    ensure(-65535, ptf.toEqualBytes(&value, sizeof(short)));
}
