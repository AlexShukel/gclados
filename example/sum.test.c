#include "sum.h"
#include "ptf.h"

// This is some test
TEST(test0) {

    ensure(1, ptf.toEqualInt(2));
}

// This is some test
TEST(test1) {
    unsigned short value = 65535;

    ensure(-65535, ptf.toEqualBytes(&value, sizeof(short)));
}

int main() {
    __ptfTest_test0(NULL).execute();
}
