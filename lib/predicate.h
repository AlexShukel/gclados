#ifndef __PTF_PREDICATE_H__
#define __PTF_PREDICATE_H__

#include <stdbool.h>

typedef char *(*GcladosValueToStringConverter)(void *value, void *options, bool pass);

typedef struct GcladosPredicate {
    bool (*execute)(void *value, void *options);
    char *usage;
    bool customOutput;
    GcladosValueToStringConverter expectedValueToString;
    GcladosValueToStringConverter receivedValueToString;
    void *options;
    void (*free)(struct GcladosPredicate *this);
} GcladosPredicate;

void gcladosFreePredicate(GcladosPredicate *predicate);

#endif