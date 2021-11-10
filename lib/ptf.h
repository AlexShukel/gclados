#ifndef __PTF_LIB_H__
#define __PTF_LIB_H__

#include "predicate.h"
#include "predicateNamespace.h"

void createPtfStatement(int line, char* filename, void* value, struct PtfPredicate predicate);

#define PTF_CONCAT(a, b) PTF_CONCAT_INNER(a, b)
#define PTF_CONCAT_INNER(a, b) a ## b
#define PTF_ENSURE_INNER(varName, value, predicate) typeof(value) (varName) = value; createPtfStatement(__LINE__, __FILE__, &(varName), predicate)

#define ensure(value, predicate) PTF_ENSURE_INNER(PTF_CONCAT(__ptfValue, __COUNTER__), value, predicate)
#define TEST(testName) void PTF_CONCAT(__ptfTest_, testName) (void)

#endif