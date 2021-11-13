#include "dynamicArray.h"

#include <string.h>

struct PtfDynamicArray createPtfDynamicArray(size_t elementSize) {
    struct PtfDynamicArray array = {
            .buffer = NULL,
            .capacity = 0,
            .length = 0,
            .elementSize = elementSize,
    };

    return array;
}

void *ptfGet(struct PtfDynamicArray *array, size_t index) {
    return array->buffer + index * array->elementSize;
}

void ptfSet(struct PtfDynamicArray *array, size_t index, void* element) {
    memcpy(array->buffer + index * array->elementSize, element, array->elementSize);
}

void ptfPush(struct PtfDynamicArray *array, void* element) {
    if(array->capacity < array->length + 1) {
        array->capacity = array->capacity == 0 ? 1 : array->capacity * 2;
        array->buffer = realloc(array->buffer, array->capacity * array->elementSize);
    }

    ptfSet(array, array->length, element);
    array->length += 1;
}
