#ifndef __PTF_TEST_PARSER_H__
#define __PTF_TEST_PARSER_H__

#include <stdlib.h>

struct ParsedTest {
    char* name;
    char* description;
};

size_t parseTestFile(char* path, struct ParsedTest *tests);

#endif
