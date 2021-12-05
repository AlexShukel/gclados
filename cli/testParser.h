// Author: Artiom Tretjakovas
// Description: This file contains functions, used for extracting test function from the file, and automating the
//              testing process.

#ifndef GCLADOS_TEST_PARSER_H
#define GCLADOS_TEST_PARSER_H

#include <stdlib.h>

// This structure contains single parsed test.
typedef struct {
    // The name of test. TEST(<name>)
    char *name;
    // The description of test (comment above test)
    char *description;
} ParsedTest;

// This structure contains parsed test file.
typedef struct {
    // The name of file.
    char *fileName;
    // Count of tests.
    size_t testCount;
    // Pointer to the array of parsed tests.
    ParsedTest *tests;
} ParsedTestFile;

// Function, which will parse test file.
// Parameters:
//     *path - full path to the test file.
// Returns:
//     ParsedTestFile instance.
ParsedTestFile parseTestFile(char *path);

#endif
