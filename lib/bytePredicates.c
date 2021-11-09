#include "bytePredicates.h"
#include "stdlib.h"

struct PtfEqualBytePredicateOptions {
    void* bytes;
    size_t count;
};

struct PtfPredicateResult ptfToEqualBytesPredicate(void* value, const struct PtfEqualBytePredicateOptions* options) {
    unsigned char* realValue = (unsigned char*) value;
    unsigned char* expectedValue = (unsigned char*) options->bytes;

    bool pass = true;
    for(int i = 0; i < options->count; i++) {
        if(realValue[i] != expectedValue[i]) {
            pass = false;
            break;
        }
    }

    char* message = calloc(options->count * 2 + 80, sizeof(char));

    char* realValueAsHex = calloc(options->count * 2 + 1, sizeof(char));
    char* expectedValueAsHex = calloc(options->count * 2 + 1, sizeof(char));

    // TODO: move to a separate function
    char valueToHex[] = "0123456789ABCDEF";

    for(int i = 0; i < options->count * 2; i += 2) {
        realValueAsHex[i] = valueToHex[realValue[i / 2] / 16];
        realValueAsHex[i + 1] = valueToHex[realValue[i / 2] % 16];
        expectedValueAsHex[i] = valueToHex[expectedValue[i / 2] / 16];
        expectedValueAsHex[i + 1] = valueToHex[expectedValue[i / 2] % 16];
    }

    // TODO: move to a separate function, to reuse it in multiple predicates
    sprintf(message, pass ? "    Expected: not 0x%s" : "    Expected: 0x%s\n    Received: 0x%s", expectedValueAsHex, realValueAsHex);

    free(realValueAsHex);
    free(expectedValueAsHex);

    struct PtfPredicateResult result = {
            .failMessage = message,
            .pass = pass,
    };

    return result;
};

struct PtfPredicate ptfToEqualBytes(void* bytes, size_t count) {
    struct PtfEqualBytePredicateOptions *options = malloc(sizeof(struct PtfEqualBytePredicateOptions));

    options->bytes = bytes;
    options->count = count;

    struct PtfPredicate predicate = {
            .options = options,
            .execute = (struct PtfPredicateResult (*)(void*, void*)) ptfToEqualBytesPredicate,
    };

    return predicate;
}