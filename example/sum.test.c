#include "sum.h"
#include "ptf.h"

// This is some test
TEST(test0) {

    ensure(sum(1, 2), ptf.toEqualInt(2));
}

// This is some test
TEST(test1) {
    unsigned short value = 65535;

    ensure(-65535, ptf.toEqualBytes(&value, sizeof(short)));
}

int main() {
    struct PtfTest tests[] = { __ptfTest_test1(NULL) };
    struct PtfTestSuite suite = createPtfTestSuite("sum.test.ts", tests, 1);
    struct PtfTestSuite suite2 = createPtfTestSuite("sum.test.ts", tests, 1);

    suite2.status = PTF_WAITING;

    printSuite(suite, true);
    printSuite(suite2, true);

}
