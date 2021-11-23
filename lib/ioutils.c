#include "ioutils.h"

#include <stdio.h>
#include <stdlib.h>

#include "colors.h"
#include "panic.h"

void gcladosPrintLineNumber(int number, bool highlight) {
    printf("\n%c %3d | ", highlight ? '>' : ' ', number);
}

char *gcladosStandardErrorMessage(bool pass, char *usage, char *expected, char *received) {
    gcladosPanic("Function is not implemented", 1);
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

        sprintf(usageColorized, predicate.usage, receivedString);

        offset += sprintf(messageBuff,
                          pass ? "  ensure(%s, gclados.not(%s));\n" : "  ensure(%s, %s);\n",
                          expectedString,
                          usageColorized);

        free(expectedString);
        free(receivedString);
        free(usageColorized);
    }

    if(predicate.expectedValueToString == NULL) {
        gcladosPanic("expectedValueToString function not specified - predicate always should print expected value.\n",
                     EXIT_FAILURE);
    }

    char *rawExpected = predicate.expectedValueToString(value, predicate.options, pass);

    if(rawExpected == NULL) {
        gcladosPanic("expectedValueToString returned NULL - predicate always should print expected value.\n",
                     EXIT_FAILURE);
    }

    if(predicate.receivedValueToString != NULL) {
        char *expected = gcladosColors.applyFlags(rawExpected, expectedValueFlags);
        offset += sprintf(messageBuff + offset, "\n    Expected: %s", expected);
        free(expected);

        char *rawReceived = predicate.receivedValueToString(value, predicate.options, pass);
        if(rawReceived != NULL) {
            char *received = gcladosColors.applyFlags(rawReceived, receivedValueFlags);
            offset += sprintf(messageBuff + offset, "\n    Received: %s", received);
            free(received);
            free(rawReceived);
        }
    } else {
        offset += sprintf(messageBuff + offset, "%s", rawExpected);
    }

    free(rawExpected);


    //    if(pass) {
    //        char *expectedBuff = calloc(256, sizeof(char));
    //        sprintf(expectedBuff, "%s", result.predicate.valueToString());
    //        char *expectedBuffColorized = gcladosColors.applyFlags(expectedBuff, expectedValueFlags);
    //        offset += sprintf(messageBuff + offset, "    Expected: %s", expectedBuffColorized);
    //
    //        free(expectedBuff);
    //        free(expectedBuffColorized);
    //    } else {
    //        char *expectedColorized = gcladosColors.applyFlags(expected, expectedValueFlags);
    //
    //        offset += sprintf(messageBuff + offset,
    //                          "    Expected: %s",
    //                          expectedColorized);
    //
    //        free(expectedColorized);
    //    }
    //
    //    char *receivedColorized = gcladosColors.applyFlags(received, receivedValueFlags);
    //    sprintf(messageBuff + offset, "\n    Received: %s", receivedColorized);
    //    free(receivedColorized);

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