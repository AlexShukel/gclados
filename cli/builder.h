// Author: Artiom Tretjakovas
// Description: This file contains functions for building tests entry file.

#ifndef GCLADOS_BUILDER_H
#define GCLADOS_BUILDER_H

#include "filenameUtils.h"
#include "testParser.h"

// Function, for building test entry file.
// Parameters:
//     *outputFile - path to the file, to which output will be written.
//     testFiles[] - array of parsed test files.
//     count       - count of parsed test files.
// Returns:
//     Temporary file name, containing generated entry code. This string should be utilized after using it, by calling
//     free.
int buildTestFile(const char *outputFile, const ParsedTestFile testFiles[], size_t count);

#endif
