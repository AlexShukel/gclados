// Author: Artiom Tretjakovas
// Description: This file contains implementations of number predicates. Function prototypes are defined in
//              "numberPredicates.h" file.

#include "numberPredicates.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMBER_PREDICATE(name, type, printFormat)                                                                      \
    char *gclados##name##ToString(type value) {                                                                        \
        char *str = calloc(100, sizeof(char));                                                                         \
        sprintf(str, printFormat, value);                                                                              \
        return str;                                                                                                    \
    }                                                                                                                  \
                                                                                                                       \
    char *gclados##name##ToStringWithPrefix(type value, char *prefix) {                                                \
        char *newBuffer = calloc(128, sizeof(char));                                                                   \
        char *convertedValue = gclados##name##ToString(value);                                                         \
                                                                                                                       \
        sprintf(newBuffer, prefix, convertedValue);                                                                    \
        free(convertedValue);                                                                                          \
                                                                                                                       \
        return newBuffer;                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    char *gclados##name##ValueToString(const type *value, void *options, bool pass) {                                  \
        return gclados##name##ToString(*value);                                                                        \
    }                                                                                                                  \
                                                                                                                       \
    GcladosPredicate gcladosCreate##name##Predicate(const type value,                                                  \
                                                    bool (*execute)(const type *value, const type *options),           \
                                                    const char *usage,                                                 \
                                                    GcladosValueToStringConverter expectedValueToString,               \
                                                    GcladosValueToStringConverter receivedValueToString) {             \
        type *options = malloc(sizeof(type));                                                                          \
        *options = value;                                                                                              \
        GcladosPredicate predicate = {                                                                                 \
                .options = options,                                                                                    \
                .usage = (char *) usage,                                                                               \
                .execute = (bool(*)(void *, void *)) execute,                                                          \
                .expectedValueToString = expectedValueToString,                                                        \
                .receivedValueToString = receivedValueToString,                                                        \
                .free = NULL,                                                                                          \
        };                                                                                                             \
                                                                                                                       \
        return predicate;                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    bool gcladosToEqual##name##Predicate(const type *value, const type *options) {                                     \
        return *value == *options;                                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    char *gcladosToEqual##name##ExpectedValue(const type *value, const type *options, bool pass) {                     \
        char *rawExpectedValue = gclados##name##ToString(*options);                                                    \
                                                                                                                       \
        if(pass) {                                                                                                     \
            char *expectedValue = calloc(128, sizeof(char));                                                           \
            sprintf(expectedValue, "not %s", rawExpectedValue);                                                        \
            free(rawExpectedValue);                                                                                    \
                                                                                                                       \
            return expectedValue;                                                                                      \
        } else {                                                                                                       \
            return rawExpectedValue;                                                                                   \
        }                                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    char *gcladosToEqual##name##ReceivedValue(const type *value, const type *options, bool pass) {                     \
        if(pass) {                                                                                                     \
            return NULL;                                                                                               \
        } else {                                                                                                       \
            return gclados##name##ToString(*value);                                                                    \
        }                                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    GcladosPredicate gcladosToEqual##name(const type value) {                                                          \
        return gcladosCreate##name##Predicate(value,                                                                   \
                                              gcladosToEqual##name##Predicate,                                         \
                                              "gclados.toEqual" #name "(%s)",                                          \
                                              (GcladosValueToStringConverter) gcladosToEqual##name##ExpectedValue,     \
                                              (GcladosValueToStringConverter) gcladosToEqual##name##ReceivedValue);    \
    }                                                                                                                  \
                                                                                                                       \
    bool gcladosToBeLessThan##name##Predicate(const type *value, const type *options) {                                \
        return *value < *options;                                                                                      \
    }                                                                                                                  \
                                                                                                                       \
    char *gcladosToBeLessThan##name##ExpectedValue(const type *value, const type *options, bool pass) {                \
        return gclados##name##ToStringWithPrefix(*options, pass ? ">= %s" : "< %s");                                   \
    }                                                                                                                  \
                                                                                                                       \
    GcladosPredicate gcladosToBeLessThan##name(const type value) {                                                     \
        return gcladosCreate##name##Predicate(                                                                         \
                value,                                                                                                 \
                gcladosToBeLessThan##name##Predicate,                                                                  \
                "gclados.toBeLessThan" #name "(%s)",                                                                   \
                (GcladosValueToStringConverter) gcladosToBeLessThan##name##ExpectedValue,                              \
                (GcladosValueToStringConverter) gclados##name##ValueToString);                                         \
    }                                                                                                                  \
                                                                                                                       \
    bool gcladosToBeGreaterThan##name##Predicate(const type *value, const type *options) {                             \
        return *value > *options;                                                                                      \
    }                                                                                                                  \
                                                                                                                       \
    char *gcladosToBeGreaterThan##name##ExpectedValue(const type *value, const type *options, bool pass) {             \
        return gclados##name##ToStringWithPrefix(*options, pass ? "<= %s" : "> %s");                                   \
    }                                                                                                                  \
                                                                                                                       \
    GcladosPredicate gcladosToBeGreaterThan##name(const type value) {                                                  \
        return gcladosCreate##name##Predicate(                                                                         \
                value,                                                                                                 \
                gcladosToBeGreaterThan##name##Predicate,                                                               \
                "gclados.toBeGreaterThan" #name "(%s)",                                                                \
                (GcladosValueToStringConverter) gcladosToBeGreaterThan##name##ExpectedValue,                           \
                (GcladosValueToStringConverter) gclados##name##ValueToString);                                         \
    }                                                                                                                  \
                                                                                                                       \
    bool gcladosToBeLessThanOrEqual##name##Predicate(const type *value, const type *options) {                         \
        return *value <= *options;                                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    char *gcladosToBeLessThanOrEqual##name##ExpectedValue(const type *value, const type *options, bool pass) {         \
        return gclados##name##ToStringWithPrefix(*options, pass ? "> %s" : "<= %s");                                   \
    }                                                                                                                  \
                                                                                                                       \
    GcladosPredicate gcladosToBeLessThanOrEqual##name(const type value) {                                              \
        return gcladosCreate##name##Predicate(                                                                         \
                value,                                                                                                 \
                gcladosToBeLessThanOrEqual##name##Predicate,                                                           \
                "gclados.toBeLessThanOrEqual" #name "(%s)",                                                            \
                (GcladosValueToStringConverter) gcladosToBeLessThanOrEqual##name##ExpectedValue,                       \
                (GcladosValueToStringConverter) gclados##name##ValueToString);                                         \
    }                                                                                                                  \
                                                                                                                       \
    bool gcladosToBeGreaterThanOrEqual##name##Predicate(const type *value, const type *options) {                      \
        return *value >= *options;                                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    char *gcladosToBeGreaterThanOrEqual##name##ExpectedValue(const type *value, const type *options, bool pass) {      \
        return gclados##name##ToStringWithPrefix(*options, pass ? "< %s" : ">= %s");                                   \
    }                                                                                                                  \
                                                                                                                       \
    GcladosPredicate gcladosToBeGreaterThanOrEqual##name(const type value) {                                           \
        return gcladosCreate##name##Predicate(                                                                         \
                value,                                                                                                 \
                gcladosToBeGreaterThanOrEqual##name##Predicate,                                                        \
                "gclados.toBeGreaterThanOrEqual" #name "(%s)",                                                         \
                (GcladosValueToStringConverter) gcladosToBeGreaterThanOrEqual##name##ExpectedValue,                    \
                (GcladosValueToStringConverter) gclados##name##ValueToString);                                         \
    }

// Predicates are described using macros, to avoid code duplication - code for all number-like values is the same, only
// type of variables differ.
// Macros are disposed at the end of file.

NUMBER_PREDICATE(Char, signed char, "%c")
NUMBER_PREDICATE(UnsignedChar, unsigned char, "%d")
NUMBER_PREDICATE(Short, signed short, "%d")
NUMBER_PREDICATE(UnsignedShort, unsigned short, "%d")
NUMBER_PREDICATE(Int, signed int, "%d")
NUMBER_PREDICATE(Unsigned, unsigned int, "%u")
NUMBER_PREDICATE(Long, signed long, "%ld")
NUMBER_PREDICATE(UnsignedLong, unsigned long, "%lu")

#undef NUMBER_PREDICATE