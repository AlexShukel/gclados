#include "bytePredicates.h"
#include "stdlib.h"

struct PtfEqualBytePredicateOptions {
    void* bytes;
    size_t count;
};

char *ptfToEqualBytesMessage(void* value, const struct PtfEqualBytePredicateOptions* options, bool pass) {
    unsigned char* realValue = (unsigned char*) value;
    unsigned char* expectedValue = (unsigned char*) options->bytes;

    char* realValueAsHex = calloc(options->count * 2 + 3, sizeof(char));
    char* expectedValueAsHex = calloc(options->count * 2 + 3, sizeof(char));

    realValueAsHex[0] = expectedValueAsHex[0] = '0';
    realValueAsHex[1] = expectedValueAsHex[1] = 'x';

    // TODO: move to a separate function
    char valueToHex[] = "0123456789ABCDEF";

    for(int i = 0; i < options->count * 2; i += 2) {
        realValueAsHex[i + 2] = valueToHex[realValue[i / 2] / 16];
        realValueAsHex[i + 3] = valueToHex[realValue[i / 2] % 16];
        expectedValueAsHex[i + 2] = valueToHex[expectedValue[i / 2] / 16];
        expectedValueAsHex[i + 3] = valueToHex[expectedValue[i / 2] % 16];
    }

    char* message = ptfStandardErrorMessage(pass, "ptf.toEqualBytes(%s, someSize)", expectedValueAsHex, realValueAsHex);

    free(realValueAsHex);
    free(expectedValueAsHex);

    return message;
}

bool ptfToEqualBytesPredicate(void* value, const struct PtfEqualBytePredicateOptions* options) {
    unsigned char* realValue = (unsigned char*) value;
    unsigned char* expectedValue = (unsigned char*) options->bytes;

    bool pass = true;
    for(int i = 0; i < options->count; i++) {
        if(realValue[i] != expectedValue[i]) {
            pass = false;
            break;
        }
    }

    return pass;
}

struct PtfPredicate ptfToEqualBytes(void* bytes, size_t count) {
    struct PtfEqualBytePredicateOptions *options = malloc(sizeof(struct PtfEqualBytePredicateOptions));

    options->bytes = bytes;
    options->count = count;

    struct PtfPredicate predicate = {
            .options = options,
            .execute = (bool (*)(void*, void*)) ptfToEqualBytesPredicate,
            .failMessage = (char *(*)(void *, void *, bool)) ptfToEqualBytesMessage,
    };

    return predicate;
}