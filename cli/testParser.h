#ifndef __PTF_TEST_PARSER_H__
#define __PTF_TEST_PARSER_H__

#include <stdlib.h>

typedef struct {
    char *name;
    char *description;
} ParsedTest;

typedef struct {
    char *fileName;
    size_t testCount;
    ParsedTest *tests;
} ParsedTestFile;

ParsedTestFile parseTestFile(char *path);

#endif
