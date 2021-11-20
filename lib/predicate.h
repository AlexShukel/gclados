#ifndef __PTF_PREDICATE_H__
#define __PTF_PREDICATE_H__

#include <stdbool.h>

typedef struct {
    bool (*execute)(void *value, void *options);
    char *(*failMessage)(void *value, void *options, bool pass);
    void *options;
} GcladosPredicate;

#endif