#include "sum.h"
#include "ptf.h"

int currentTEST(int asdf) {
    return 0;
};

// Should be equal to int
TEST(test0) {
    int sum2 = 0;
    for(int i = 0; i < 1000000000; ++i) {
        sum2 += i;
    }

    const char fakeTest[] = " TEST(insideString) ";

    ensure(sum(1, 2), ptf.toEqualInt(2));
}

// Should be equal to bytes
TEST(test1) {
    unsigned short value = 65535;
    int sum2 = 0;
    for(int i = 0; i < 1000000000; ++i) {
        sum2 += i;
    }

    ensure(-65535, ptf.toEqualBytes(&value, sizeof(short)));
}
/*
int main() {
    struct PtfTest ptfTests1[] = {
            __ptfTest_test0("Should be equal to int"),
            __ptfTest_test1("Should be equal to bytes"),
    };
    struct PtfTestSuite ptfTestSuite1 = createPtfTestSuite("../../example/sum.test.c", ptfTests1, 2);
    struct PtfTestSuite ptfTestSuites[] = {
            ptfTestSuite1,
    };
    runPtfTestSuites(ptfTestSuites, 1);
}
*/