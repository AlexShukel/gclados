#ifndef GCLADOS_DYNAMIC_ARRAY_H
#define GCLADOS_DYNAMIC_ARRAY_H

#include <stdlib.h>

typedef struct {
    void **buffer;
    size_t capacity;
    size_t length;
} GcladosDynamicArray;

GcladosDynamicArray *gcladosCreateDynamicArray();
void *gcladosGet(GcladosDynamicArray *array, size_t index);
void gcladosPush(GcladosDynamicArray *array, void *element);
void gcladosSet(GcladosDynamicArray *array, size_t index, void *element);
void gcladosFreeDynamicArray(GcladosDynamicArray *array);

#endif
