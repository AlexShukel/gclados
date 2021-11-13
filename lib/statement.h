#ifndef __PTF_STATEMENT_H__
#define __PTF_STATEMENT_H__

#include "predicate.h"
#include "test.h"

void createPtfStatement(int line, char* filePath, void* value, struct PtfPredicate predicate);

#endif
