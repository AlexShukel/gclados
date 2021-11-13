#ifndef __PTF_TEST_PARSER_H__
#define __PTF_TEST_PARSER_H__

#include <stdlib.h>

struct ParsedTest {
    char *name;
    char *description;
};

struct ParsedTestFile {
    char *fileName;
    size_t testCount;
    struct ParsedTest *tests;
};

struct ParsedTestFile parseTestFile(char* path);

#endif
