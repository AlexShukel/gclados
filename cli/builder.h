#ifndef __PTF_BUILDER_H__
#define __PTF_BUILDER_H__

#include "filenameUtils.h"
#include "testParser.h"

char *buildTestFile(ParsedTestFile *testFiles, size_t count);

#endif
