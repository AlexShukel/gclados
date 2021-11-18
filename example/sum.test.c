#include "sum.h"
#include "gclados.h"

int currentTEST(int asdf) {
    return 0;
};

// Should be equal to int
TEST(test0) {
    const char fakeTest[] = " TEST(insideString) ";

    sum(1, 2);
}

// Should be equal to bytes
TEST(test1) {
    unsigned short value = 65535;
    ensure(-65535, gclados.toEqualBytes(&value, sizeof(short)));
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