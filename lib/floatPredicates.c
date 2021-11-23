#include "floatPredicates.h"

#include <float.h>
#include <stdio.h>
#include <stdlib.h>

#include "ioutils.h"

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
    type gclados##name##Abs(const type value) {                                                                        \
        return value < 0 ? -value : value;                                                                             \
    }                                                                                                                  \
                                                                                                                       \
    bool gcladosToEqual##name##Predicate(const type *value, const Gclados##name##Options *options) {                   \
        return gclados##name##Abs(*value - options->value) <= options->precision / 2.0f;                               \
    }                                                                                                                  \
                                                                                                                       \
    char *gcladosToEqual##name##Message(const type *value, const Gclados##name##Options *options, bool pass) {         \
        char *rawExpectedValue = gclados##name##ToString(options->value);                                              \
        char *precisionAsString = gclados##name##ToString(options->precision);                                         \
        char expectedValue[200];                                                                                       \
                                                                                                                       \
        sprintf(expectedValue,                                                                                         \
                "%s ~%s",                                                                                              \
                rawExpectedValue,                                                                                      \
                options->precision == epsilon ? "epsilon" : precisionAsString);                                        \
                                                                                                                       \
        free(precisionAsString);                                                                                       \
        free(rawExpectedValue);                                                                                        \
                                                                                                                       \
        char *receivedValue = gclados##name##ToString(*value);                                                         \
                                                                                                                       \
        char *message = gcladosStandardErrorMessage(pass,                                                              \
                                                    "gclados.toEqual" #name "(%s, precision)",                         \
                                                    expectedValue,                                                     \
                                                    receivedValue);                                                    \
                                                                                                                       \
        free(receivedValue);                                                                                           \
                                                                                                                       \
        return message;                                                                                                \
    }                                                                                                                  \
                                                                                                                       \
    GcladosPredicate gcladosToEqual##name(const type value, const type precision) {                                    \
        Gclados##name##Options *options = malloc(sizeof(Gclados##name##Options));                                      \
        options->value = value;                                                                                        \
        options->precision = precision;                                                                                \
                                                                                                                       \
        GcladosPredicate predicate = {                                                                                 \
                .execute = (bool(*)(void *, void *)) gcladosToEqual##name##Predicate,                                  \
                .failMessage = (char *(*) (void *, void *, bool) ) gcladosToEqual##name##Message,                      \
                .options = options,                                                                                    \
        };                                                                                                             \
                                                                                                                       \
        return predicate;                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    bool gcladosToBeLessThan##name##Predicate(const type *value, const type *options) {                                \
        return *value < *options;                                                                                      \
    }                                                                                                                  \
                                                                                                                       \
    char *gcladosToBeLessThan##name##Message(const type *value, const type *options, bool pass) {                      \
        char *rawExpectedValue = gclados##name##ToString(*options);                                                    \
        char expectedValue[100];                                                                                       \
                                                                                                                       \
        sprintf(expectedValue, "< %s", rawExpectedValue);                                                              \
        free(rawExpectedValue);                                                                                        \
                                                                                                                       \
        char *receivedValue = gclados##name##ToString(*value);                                                         \
                                                                                                                       \
        char *message =                                                                                                \
                gcladosStandardErrorMessage(pass, "gclados.toBeLessThan" #name "(%s)", expectedValue, receivedValue);  \
                                                                                                                       \
        free(receivedValue);                                                                                           \
                                                                                                                       \
        return message;                                                                                                \
    }                                                                                                                  \
                                                                                                                       \
    GcladosPredicate gcladosToBeLessThan##name(const type value) {                                                     \
        type *options = malloc(sizeof(type));                                                                          \
        *options = value;                                                                                              \
                                                                                                                       \
        GcladosPredicate predicate = {                                                                                 \
                .execute = (bool(*)(void *, void *)) gcladosToBeLessThan##name##Predicate,                             \
                .failMessage = (char *(*) (void *, void *, bool) ) gcladosToBeLessThan##name##Message,                 \
                .options = options,                                                                                    \
        };                                                                                                             \
                                                                                                                       \
        return predicate;                                                                                              \
    }                                                                                                                  \
    bool gcladosToBeGreaterThan##name##Predicate(const type *value, const type *options) {                             \
        return *value > *options;                                                                                      \
    }                                                                                                                  \
                                                                                                                       \
    char *gcladosToBeGreaterThan##name##Message(const type *value, const type *options, bool pass) {                   \
        char *rawExpectedValue = gclados##name##ToString(*options);                                                    \
        char expectedValue[100];                                                                                       \
                                                                                                                       \
        sprintf(expectedValue, "> %s", rawExpectedValue);                                                              \
        free(rawExpectedValue);                                                                                        \
                                                                                                                       \
        char *receivedValue = gclados##name##ToString(*value);                                                         \
                                                                                                                       \
        char *message =                                                                                                \
                gcladosStandardErrorMessage(pass, "gclados.toBeLessThan" #name "(%s)", expectedValue, receivedValue);  \
                                                                                                                       \
        free(receivedValue);                                                                                           \
                                                                                                                       \
        return message;                                                                                                \
    }                                                                                                                  \
                                                                                                                       \
    GcladosPredicate gcladosToBeGreaterThan##name(const type value) {                                                  \
        type *options = malloc(sizeof(type));                                                                          \
        *options = value;                                                                                              \
                                                                                                                       \
        GcladosPredicate predicate = {                                                                                 \
                .execute = (bool(*)(void *, void *)) gcladosToBeLessThan##name##Predicate,                             \
                .failMessage = (char *(*) (void *, void *, bool) ) gcladosToBeLessThan##name##Message,                 \
                .options = options,                                                                                    \
        };                                                                                                             \
                                                                                                                       \
        return predicate;                                                                                              \
    }

FLOAT_PREDICATE(Float, float, "%f", FLT_EPSILON)
FLOAT_PREDICATE(Double, double, "%lf", DBL_EPSILON)
FLOAT_PREDICATE(LongDouble, long double, "%Lf", LDBL_EPSILON)

#undef FLOAT_PREDICATE