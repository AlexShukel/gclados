#include "numberPredicates.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "ioutils.h"

#define NUMBER_PREDICATE(name, type, printFormat)                                                                      \
    char *gclados##name##ToString(type a) {                                                                            \
        char *str = calloc(100, sizeof(char));                                                                         \
        sprintf(str, printFormat, a);                                                                                  \
        return str;                                                                                                    \
    }                                                                                                                  \
                                                                                                                       \
    GcladosPredicate gcladosCreate##name##Predicate(                                                                   \
            const type value,                                                                                          \
            bool (*execute)(const type *value, const type *options),                                                   \
            char *(*message)(const type *value, const type *options, bool pass)) {                                     \
        type *options = malloc(sizeof(type));                                                                          \
        *options = value;                                                                                              \
        GcladosPredicate predicate = {                                                                                 \
                .options = options,                                                                                    \
                .execute = (bool(*)(void *, void *)) execute,                                                          \
                .failMessage = (char *(*) (void *, void *, bool) ) message,                                            \
        };                                                                                                             \
                                                                                                                       \
        return predicate;                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    char *gclados##name##Message(const type *options,                                                                  \
                                 const type *value,                                                                    \
                                 bool pass,                                                                            \
                                 char *usage,                                                                          \
                                 char *expectedFormat) {                                                               \
        char *rawExpectedValueAsString = gclados##name##ToString(*options);                                            \
        char *receivedValueAsString = gclados##name##ToString(*value);                                                 \
                                                                                                                       \
        char expectedValueAsString[100];                                                                               \
        sprintf(expectedValueAsString, expectedFormat, rawExpectedValueAsString);                                      \
        free(rawExpectedValueAsString);                                                                                \
                                                                                                                       \
        char *message = gcladosStandardErrorMessage(pass, usage, expectedValueAsString, receivedValueAsString);        \
                                                                                                                       \
        free(receivedValueAsString);                                                                                   \
                                                                                                                       \
        return message;                                                                                                \
    }                                                                                                                  \
                                                                                                                       \
    bool gcladosToEqual##name##Predicate(const type *value, const type *options) {                                     \
        return *value == *options;                                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    char *gcladosToEqual##name##Message(const type *value, const type *options, bool pass) {                           \
        return gclados##name##Message(options, value, pass, "gclados.toEqual" #name "(%s)", "%s");                     \
    }                                                                                                                  \
                                                                                                                       \
    GcladosPredicate gcladosToEqual##name(const type value) {                                                          \
        return gcladosCreate##name##Predicate(value, gcladosToEqual##name##Predicate, gcladosToEqual##name##Message);  \
    }                                                                                                                  \
                                                                                                                       \
    bool gcladosToBeLessThan##name##Predicate(const type *value, const type *options) {                                \
        return *value < *options;                                                                                      \
    }                                                                                                                  \
                                                                                                                       \
    char *gcladosToBeLessThan##name##Message(const type *value, const type *options, bool pass) {                      \
        return gclados##name##Message(options, value, pass, "gclados.toBeLessThan" #name "(%s)", "< %s");              \
    }                                                                                                                  \
                                                                                                                       \
    GcladosPredicate gcladosToBeLessThan##name(const type value) {                                                     \
        return gcladosCreate##name##Predicate(value,                                                                   \
                                              gcladosToBeLessThan##name##Predicate,                                    \
                                              gcladosToBeLessThan##name##Message);                                     \
    }                                                                                                                  \
                                                                                                                       \
    bool gcladosToBeGreaterThan##name##Predicate(const type *value, const type *options) {                             \
        return *value > *options;                                                                                      \
    }                                                                                                                  \
                                                                                                                       \
    char *gcladosToBeGreaterThan##name##Message(const type *value, const type *options, bool pass) {                   \
        return gclados##name##Message(options, value, pass, "gclados.toBeGreaterThan" #name "(%s)", "> %s");           \
    }                                                                                                                  \
                                                                                                                       \
    GcladosPredicate gcladosToBeGreaterThan##name(const type value) {                                                  \
        return gcladosCreate##name##Predicate(value,                                                                   \
                                              gcladosToBeGreaterThan##name##Predicate,                                 \
                                              gcladosToBeGreaterThan##name##Message);                                  \
    }                                                                                                                  \
                                                                                                                       \
    bool gcladosToBeLessThanOrEqual##name##Predicate(const type *value, const type *options) {                         \
        return *value <= *options;                                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    char *gcladosToBeLessThanOrEqual##name##Message(const type *value, const type *options, bool pass) {               \
        return gclados##name##Message(options, value, pass, "gclados.toBeLessThanOrEqual" #name "(%s)", "<= %s");      \
    }                                                                                                                  \
                                                                                                                       \
    GcladosPredicate gcladosToBeLessThanOrEqual##name(const type value) {                                              \
        return gcladosCreate##name##Predicate(value,                                                                   \
                                              gcladosToBeLessThanOrEqual##name##Predicate,                             \
                                              gcladosToBeLessThanOrEqual##name##Message);                              \
    }                                                                                                                  \
                                                                                                                       \
    bool gcladosToBeGreaterThanOrEqual##name##Predicate(const type *value, const type *options) {                      \
        return *value >= *options;                                                                                     \
    }                                                                                                                  \
                                                                                                                       \
    char *gcladosToBeGreaterThanOrEqual##name##Message(const type *value, const type *options, bool pass) {            \
        return gclados##name##Message(options, value, pass, "gclados.toBeGreaterThanOrEqual" #name "(%s)", ">= %s");   \
    }                                                                                                                  \
                                                                                                                       \
    GcladosPredicate gcladosToBeGreaterThanOrEqual##name(const type value) {                                           \
        return gcladosCreate##name##Predicate(value,                                                                   \
                                              gcladosToBeGreaterThanOrEqual##name##Predicate,                          \
                                              gcladosToBeGreaterThanOrEqual##name##Message);                           \
    }


NUMBER_PREDICATE(Char, signed char, "%c")
NUMBER_PREDICATE(UnsignedChar, unsigned char, "%d")
NUMBER_PREDICATE(Short, signed short, "%d")
NUMBER_PREDICATE(UnsignedShort, unsigned short, "%d")
NUMBER_PREDICATE(Int, signed int, "%d")
NUMBER_PREDICATE(Unsigned, unsigned int, "%u")
NUMBER_PREDICATE(Long, signed long, "%ld")
NUMBER_PREDICATE(UnsignedLong, unsigned long, "%lu")
NUMBER_PREDICATE(Float, float, "%f")
NUMBER_PREDICATE(Double, double, "%lf")
NUMBER_PREDICATE(LongDouble, long double, "%Lf")

#undef NUMBER_PREDICATE