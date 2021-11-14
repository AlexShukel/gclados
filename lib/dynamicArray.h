#ifndef __PTF_DYNAMIC_ARRAY_H__
#define __PTF_DYNAMIC_ARRAY_H__

#include "stdlib.h"

struct PtfDynamicArray {
    size_t elementSize;
    char* buffer;
    size_t capacity;
    size_t length;
};

struct PtfDynamicArray createPtfDynamicArray(size_t elementSize);
void *ptfGet(struct PtfDynamicArray *array, size_t index);
void ptfPush(struct PtfDynamicArray *array, void* element);
void ptfSet(struct PtfDynamicArray *array, size_t index, void* element);
void ptfFreeDynamicArray(struct PtfDynamicArray *array);

#endif
