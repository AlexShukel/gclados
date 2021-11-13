#ifndef __PTF_BYTE_PREDICATES_H__
#define __PTF_BYTE_PREDICATES_H__

#include "predicate.h"
#include "stdio.h"
#include "ioutils.h"

struct PtfPredicate ptfToEqualBytes(void* bytes, size_t count);

#endif
