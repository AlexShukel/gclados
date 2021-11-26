#include "ioutils.h"

#include <stdio.h>
#include <stdlib.h>

#include "colors.h"
#include "panic.h"

void gcladosPrintLineNumber(int number, bool highlight) {
    printf("\n%c %3d | ", highlight ? '>' : ' ', number);
}

void gcladosPrintFileLines(FILE *file, int lineBegin, int lineEnd, int highlightedLine) {
    if(lineBegin < 1 || lineEnd < 1) {
        gcladosPanic("Begin and end lines should be not less than 1", EXIT_FAILURE);
    }

    int currentCharacter, currentLine = 1;

    while(currentCharacter = fgetc(file),
          currentLine += (currentCharacter == '\n'),
          currentLine < lineBegin && currentCharacter != EOF)
        ;

    gcladosPrintLineNumber(currentLine, currentLine == highlightedLine);

    while(currentCharacter = fgetc(file), currentLine <= lineEnd && currentCharacter != EOF) {
        if(currentCharacter == '\n') {
            if(++currentLine <= lineEnd) {
                gcladosPrintLineNumber(currentLine, currentLine == highlightedLine);
            }
        } else if(currentCharacter == '\r') {
            continue;
        } else {
            putc(currentCharacter, stdout);
        }
    }

    putc('\n', stdout);
}

void gcladosPrintProgress(FILE *file, double percentage, size_t width) {
    size_t progressBufferLength = width + 3;
    char progressBuffer[width + progressBufferLength];

    progressBuffer[0] = '[';
    progressBuffer[progressBufferLength - 2] = ']';
    progressBuffer[progressBufferLength - 1] = '\0';

    size_t center = (size_t) ((double) width * percentage) + 1;

    for(size_t i = 1; i < width + 1; i++) {
        progressBuffer[i] = i <= center ? '#' : ' ';
    }

    fprintf(file, "%s", progressBuffer);
}

char *gcladosGetFailedStatementMessage(bool pass, GcladosPredicate predicate, void *value) {
    const GcladosAnsiFlags expectedValueFlags =
            gcladosColors.createFlags(2, gcladosColors.foregroundColor(GCLADOS_GREEN), gcladosColors.bold());
    const GcladosAnsiFlags receivedValueFlags =
            gcladosColors.createFlags(2, gcladosColors.foregroundColor(GCLADOS_RED), gcladosColors.bold());

    char *messageBuff = calloc(1024, sizeof(char));
    int offset = 0;

    if(predicate.usage != NULL) {
        char *expectedString = gcladosColors.applyFlags("expected", expectedValueFlags);
        char *receivedString = gcladosColors.applyFlags("received", receivedValueFlags);

        char *usageColorized = calloc(256, sizeof(char));

        sprintf(usageColorized, predicate.usage, expectedString);

        offset += sprintf(messageBuff,
                          pass ? "  ensure(%s, gclados.not(%s));\n" : "  ensure(%s, %s);\n",
                          receivedString,
                          usageColorized);

        free(expectedString);
        free(receivedString);
        free(usageColorized);
    }

    if(predicate.expectedValueToString != NULL) {
        char *rawExpected = predicate.expectedValueToString(value, predicate.options, pass);

        if(predicate.customOutput) {
            offset += sprintf(messageBuff + offset, "\n%s", rawExpected);
        } else {
            char *expected = gcladosColors.applyFlags(rawExpected, expectedValueFlags);
            offset += sprintf(messageBuff + offset, "\n    Expected: %s", expected);
            free(expected);
        }

        free(rawExpected);
    }

    if(!predicate.customOutput && predicate.receivedValueToString != NULL) {
        char *rawReceived = predicate.receivedValueToString(value, predicate.options, pass);
        char *received = gcladosColors.applyFlags(rawReceived, receivedValueFlags);
        offset += sprintf(messageBuff + offset, "\n    Received: %s", received);
        free(received);
        free(rawReceived);
    }

    gcladosColors.freeFlags(expectedValueFlags);
    gcladosColors.freeFlags(receivedValueFlags);

    return messageBuff;
}

const char gcladosHexValues[] = "0123456789ABCDEF";

char *gcladosConvertToHex(void *ptr, size_t size) {
    char *hexString = calloc(size * 2 + 3, sizeof(char));
    unsigned char *castedPtr = (unsigned char *) ptr;

    hexString[0] = '0';
    hexString[1] = 'x';

    for(int i = 0; i < size * 2; i += 2) {
        hexString[size * 2 - i + 1] = gcladosHexValues[castedPtr[i / 2] % 16];
        hexString[size * 2 - i] = gcladosHexValues[castedPtr[i / 2] / 16];
    }

    return hexString;
}