#include "sum.h"
#include "ptf.h"

// This is some test
TEST {
    unsigned short value = 65535;

    ensure(-65535, ptf.toEqualBytes(&value, sizeof(short)));
}

int main() {
    __ptfTest0();
}