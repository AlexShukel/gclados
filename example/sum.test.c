#include "sum.h"
#include "ptf.h"

// This is some test
TEST {
    ensure(sum(1, 2), ptf.toEqualInt(4));
}

int main() {
    __ptfTest0();
}