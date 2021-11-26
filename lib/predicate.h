#ifndef __PTF_PREDICATE_H__
#define __PTF_PREDICATE_H__

#include <stdbool.h>

typedef char *(*GcladosValueToStringConverter)(void *value, void *options, bool pass);

typedef struct {
    bool (*execute)(void *value, void *options);
    char *usage;
    bool customOutput;
    GcladosValueToStringConverter expectedValueToString;
    GcladosValueToStringConverter receivedValueToString;
    void *options;
} GcladosPredicate;

#endif