#ifndef __PTF_BUILDER_H__
#define __PTF_BUILDER_H__

#include "testParser.h"
#include "filenameUtils.h"

char *buildTestFile(struct ParsedTestFile* testFiles, size_t count);

#endif
