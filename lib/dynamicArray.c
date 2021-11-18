#include "dynamicArray.h"

#include <string.h>

GcladosDynamicArray gcladosCreateDynamicArray(size_t elementSize) {
    GcladosDynamicArray array = {
            .buffer = NULL,
            .capacity = 0,
            .length = 0,
            .elementSize = elementSize,
    };

    return array;
}

void *gcladosGet(GcladosDynamicArray *array, size_t index) {
    return array->buffer + index * array->elementSize;
}

void gcladosSet(GcladosDynamicArray *array, size_t index, void* element) {
    memcpy(array->buffer + index * array->elementSize, element, array->elementSize);
}

void gcladosPush(GcladosDynamicArray *array, void* element) {
    if(array->capacity < array->length + 1) {
        array->capacity = array->capacity == 0 ? 1 : array->capacity * 2;
        array->buffer = realloc(array->buffer, array->capacity * array->elementSize);
    }

    gcladosSet(array, array->length, element);
    array->length += 1;
}

void gcladosFreeDynamicArray(GcladosDynamicArray *array) {
    free(array->buffer);
    free(array);
}
