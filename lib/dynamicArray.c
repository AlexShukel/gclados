// Author: Artiom Tretjakovas
// Description: This file contains implementation of DynamicArray. Function prototypes are described in "dynamicArray.h"
//              file.

#include "dynamicArray.h"

#include <signal.h>
#include <string.h>

#include "panic.h"

GcladosDynamicArray *gcladosCreateDynamicArray() {
    GcladosDynamicArray *array = malloc(sizeof(GcladosDynamicArray));

    array->buffer = NULL;
    array->capacity = 0;
    array->length = 0;

    return array;
}

void *gcladosGet(GcladosDynamicArray *array, size_t index) {
    if(index >= array->length || index < 0) {
        gcladosPanic("Index out of bounds", SIGABRT);
    }

    return array->buffer[index];
}

void gcladosSet(GcladosDynamicArray *array, size_t index, void *element) {
    if(index >= array->length || index < 0) {
        gcladosPanic("Index out of bounds", SIGABRT);
    }

    array->buffer[index] = element;
}

void gcladosPush(GcladosDynamicArray *array, void *element) {
    if(array->capacity < array->length + 1) {
        // Increasing array capacity.
        array->capacity = array->capacity == 0 ? 1 : array->capacity * 2;
        array->buffer = realloc(array->buffer, array->capacity * sizeof(void *));
    }

    size_t lastIndex = array->length;
    array->length += 1;
    gcladosSet(array, lastIndex, element);
}

void gcladosFreeDynamicArray(GcladosDynamicArray *array) {
    if(array != NULL) {
        free(array->buffer);
        free(array);
    }
}
