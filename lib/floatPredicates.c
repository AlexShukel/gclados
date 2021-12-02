#include "floatPredicates.h"

#include <float.h>
#include <stdio.h>
#include <stdlib.h>

#define FLOAT_PREDICATE(name, type, printFormat, epsilon)                                                              \
                                                                                                                       \
    typedef struct {                                                                                                   \
        type value;                                                                                                    \
        type precision;                                                                                                \
    } Gclados##name##Options;                                                                                          \
                                                                                                                       \
    char *gclados##name##ToString(type a) {                                                                            \
        char *str = calloc(100, sizeof(char));                                                                         \
        sprintf(str, printFormat, a);                                                                                  \
        return str;                                                                                                    \
    }                                                                                                                  \
                                                                                                                       \
    char *gclados##name##ValueToString(const type *value, const void *options, bool pass) {                            \
        return gclados##name##ToString(*value);                                                                        \
    }                                                                                                                  \
                                                                                                                       \
    type gclados##name##Abs(const type value) {                                                                        \
        return value < 0 ? -value : value;                                                                             \
    }                                                                                                                  \
                                                                                                                       \
    bool gcladosToEqual##name##Predicate(const type *value, const Gclados##name##Options *options) {                   \
        return gclados##name##Abs(*value - options->value) <= options->precision / 2.0f;                               \
    }                                                                                                                  \
                                                                                                                       \
    char *gcladosToEqual##name##ExpectedValue(const type *value, const Gclados##name##Options *options, bool pass) {   \
        char *rawExpectedValue = gclados##name##ToString(options->value);                                              \
        char *precisionAsString = gclados##name##ToString(options->precision);                                         \
        char *expectedValue = calloc(256, sizeof(char));                                                               \
                                                                                                                       \
        sprintf(expectedValue,                                                                                         \
                pass ? "not %s ~%s" : "%s ~%s",                                                                        \
                rawExpectedValue,                                                                                      \
                options->precision == epsilon ? "epsilon" : precisionAsString);                                        \
                                                                                                                       \
        free(precisionAsString);                                                                                       \
        free(rawExpectedValue);                                                                                        \
                                                                                                                       \
        return expectedValue;                                                                                          \
    }                                                                                                                  \
                                                                                                                       \
    char *gcladosToEqual##name##ReceivedValue(const type *value, const Gclados##name##Options *options, bool pass) {   \
        if(pass) {                                                                                                     \
            return NULL;                                                                                               \
        } else {                                                                                                       \
            return gclados##name##ToString(*value);                                                                    \
        }                                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    GcladosPredicate gcladosToEqual##name(const type value, const type precision) {                                    \
        Gclados##name##Options *options = malloc(sizeof(Gclados##name##Options));                                      \
        options->value = value;                                                                                        \
        options->precision = precision;                                                                                \
                                                                                                                       \
        GcladosPredicate predicate = {                                                                                 \
                .execute = (bool(*)(void *, void *)) gcladosToEqual##name##Predicate,                                  \
                .usage = "gclados.toEqual" #name "(%s, precision)",                                                    \
                .expectedValueToString = (GcladosValueToStringConverter) gcladosToEqual##name##ExpectedValue,          \
                .receivedValueToString = (GcladosValueToStringConverter) gcladosToEqual##name##ReceivedValue,          \
                .options = options,                                                                                    \
                .free = NULL,                                                                                          \
        };                                                                                                             \
                                                                                                                       \
        return predicate;                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    bool gcladosToBeLessThan##name##Predicate(const type *value, const type *options) {                                \
        return *value < *options;                                                                                      \
    }                                                                                                                  \
                                                                                                                       \
    char *gcladosToBeLessThan##name##ExpectedValue(const type *value, const type *options, bool pass) {                \
        char *rawExpectedValue = gclados##name##ToString(*options);                                                    \
        char *expectedValue = calloc(128, sizeof(char));                                                               \
                                                                                                                       \
        sprintf(expectedValue, pass ? ">= %s" : "< %s", rawExpectedValue);                                             \
        free(rawExpectedValue);                                                                                        \
                                                                                                                       \
        return expectedValue;                                                                                          \
    }                                                                                                                  \
                                                                                                                       \
    GcladosPredicate gcladosToBeLessThan##name(const type value) {                                                     \
        type *options = malloc(sizeof(type));                                                                          \
        *options = value;                                                                                              \
                                                                                                                       \
        GcladosPredicate predicate = {                                                                                 \
                .execute = (bool(*)(void *, void *)) gcladosToBeLessThan##name##Predicate,                             \
                .usage = "gclados.toBeLessThan" #name "(%s)",                                                          \
                .expectedValueToString = (GcladosValueToStringConverter) gcladosToBeLessThan##name##ExpectedValue,     \
                .receivedValueToString = (GcladosValueToStringConverter) gclados##name##ValueToString,                 \
                .options = options,                                                                                    \
                .free = NULL,                                                                                          \
        };                                                                                                             \
                                                                                                                       \
        return predicate;                                                                                              \
    }                                                                                                                  \
    bool gcladosToBeGreaterThan##name##Predicate(const type *value, const type *options) {                             \
        return *value > *options;                                                                                      \
    }                                                                                                                  \
                                                                                                                       \
    char *gcladosToBeGreaterThan##name##ExpectedValue(const type *value, const type *options, bool pass) {             \
        char *rawExpectedValue = gclados##name##ToString(*options);                                                    \
        char *expectedValue = calloc(128, sizeof(char));                                                               \
                                                                                                                       \
        sprintf(expectedValue, pass ? "<= %s" : "> %s", rawExpectedValue);                                             \
        free(rawExpectedValue);                                                                                        \
                                                                                                                       \
        return expectedValue;                                                                                          \
    }                                                                                                                  \
                                                                                                                       \
    GcladosPredicate gcladosToBeGreaterThan##name(const type value) {                                                  \
        type *options = malloc(sizeof(type));                                                                          \
        *options = value;                                                                                              \
                                                                                                                       \
        GcladosPredicate predicate = {                                                                                 \
                .execute = (bool(*)(void *, void *)) gcladosToBeGreaterThan##name##Predicate,                          \
                .usage = "gclados.toBeGreaterThan" #name "(%s)",                                                       \
                .expectedValueToString = (GcladosValueToStringConverter) gcladosToBeGreaterThan##name##ExpectedValue,  \
                .receivedValueToString = (GcladosValueToStringConverter) gclados##name##ValueToString,                 \
                .options = options,                                                                                    \
                .free = NULL,                                                                                          \
        };                                                                                                             \
                                                                                                                       \
        return predicate;                                                                                              \
    }

FLOAT_PREDICATE(Float, float, "%f", FLT_EPSILON)
FLOAT_PREDICATE(Double, double, "%lf", DBL_EPSILON)
FLOAT_PREDICATE(LongDouble, long double, "%Lf", LDBL_EPSILON)

#undef FLOAT_PREDICATE