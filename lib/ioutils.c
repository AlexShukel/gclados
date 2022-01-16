// Author: Artiom Tretjakovas
// Description: This file contains implementation of helper functions for manipulating stdio. Function prototypes are in
//              "ioutils.h" file.

#include "ioutils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "panic.h"

void gcladosPrintLineNumber(int number, bool highlight) {
    printf("\n%c %3d | ", highlight ? '>' : ' ', number);
}

void gcladosPrintFileLines(FILE *file, int lineBegin, int lineEnd, int highlightedLine) {
    // Throw error if lines does not exist.
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

const size_t GCLADOS_MAX_PROGRESS_TEXT_WIDTH = 6;
const size_t GCLADOS_MIN_PROGRESS_WIDTH = GCLADOS_MAX_PROGRESS_TEXT_WIDTH + 2;

void gcladosPrintMonochromaticProgress(FILE *file, double percentage, size_t center, size_t width) {
    char progressBuffer[width + 1];
    progressBuffer[width] = '\0';

    memset(progressBuffer, '=', center);
    memset(progressBuffer + center, ' ', width - center);

    char progressText[GCLADOS_MAX_PROGRESS_TEXT_WIDTH + 1];
    memset(progressText, 0, GCLADOS_MAX_PROGRESS_TEXT_WIDTH + 1);

    sprintf(progressText, " %-3d%% ", (int) (percentage * 100));

    size_t textOffset = (width - strlen(progressText)) / 2;
    char savedSymbol = progressBuffer[textOffset + strlen(progressText)];
    sprintf(progressBuffer + textOffset, "%s", progressText);
    progressBuffer[textOffset + strlen(progressText)] = savedSymbol;

    fprintf(file, "[%s]", progressBuffer);
}

void gcladosPrintColoredProgress(FILE *file, double percentage, size_t center, size_t width) {
    char buffer[width + 1];
    memset(buffer, ' ', width);
    buffer[width + 1] = '\0';

    size_t textOffset = (width - 4) / 2;

    sprintf(buffer + textOffset, "%-3d%%", (int) (percentage * 100));
    buffer[textOffset + 4] = ' ';

    char progress[center + 1];
    progress[center] = '\0';
    strncpy(progress, buffer, center);

    GcladosAnsiFlags progressStyle = gcladosColors.createFlags(2,
                                                               gcladosColors.backgroundColor(GCLADOS_GREEN),
                                                               gcladosColors.foregroundColor(GCLADOS_BLACK));

    char *coloredProgress = gcladosColors.applyFlags(progress, progressStyle);
    fprintf(file, "%s", coloredProgress);

    free(coloredProgress);
    gcladosColors.freeFlags(progressStyle);

    if(center >= width) {
        return;
    }

    GcladosAnsiFlags backgroundStyle = gcladosColors.createFlags(2,
                                                                 gcladosColors.backgroundColor(GCLADOS_WHITE),
                                                                 gcladosColors.foregroundColor(GCLADOS_BLACK));

    size_t backgroundWidth = width - center;
    char background[backgroundWidth + 1];
    background[backgroundWidth] = '\0';
    strncpy(background, buffer + center, backgroundWidth);

    char *coloredBackground = gcladosColors.applyFlags(background, backgroundStyle);
    fprintf(file, "%s", coloredBackground);

    free(coloredBackground);
    gcladosColors.freeFlags(backgroundStyle);
}

void gcladosPrintProgress(FILE *file, double percentage, size_t width) {
    if(width < GCLADOS_MIN_PROGRESS_WIDTH) {
        gcladosPanic("Progress bar is too short.", EXIT_FAILURE);
    }

    size_t center = (size_t) ((double) (width - 1) * percentage) + 1;
    if(gcladosColors.colorsSupported()) {
        gcladosPrintColoredProgress(file, percentage, center, width);
    } else {
        gcladosPrintMonochromaticProgress(file, percentage, center, width);
    }
}

GcladosAnsiFlags gcladosDefaultFailureStyle() {
    return gcladosColors.createFlags(2, gcladosColors.foregroundColor(GCLADOS_RED), gcladosColors.bold());
}

GcladosAnsiFlags gcladosDefaultSuccessStyle() {
    return gcladosColors.createFlags(2, gcladosColors.foregroundColor(GCLADOS_GREEN), gcladosColors.bold());
}

char *gcladosGetFailedStatementMessage(bool pass, GcladosPredicate predicate, void *value) {
    const GcladosAnsiFlags expectedValueFlags = gcladosDefaultSuccessStyle();
    const GcladosAnsiFlags receivedValueFlags = gcladosDefaultFailureStyle();

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
        if(rawReceived != NULL) {
            char *received = gcladosColors.applyFlags(rawReceived, receivedValueFlags);
            offset += sprintf(messageBuff + offset, "\n    Received: %s", received);
            free(received);
            free(rawReceived);
        }
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

void gcladosPrintTime(clock_t time) {
    int minutes = (int) (time / (CLOCKS_PER_SEC * 60ul));
    int seconds = (int) (time / CLOCKS_PER_SEC) % 60;
    int milliseconds = (int) (time / (CLOCKS_PER_SEC / 1000)) % 1000;
    int nanoseconds = (int) (time % (CLOCKS_PER_SEC / 1000));

    int sections[] = {minutes, seconds, milliseconds, nanoseconds};
    char *sectionNames[] = {"%ld minutes", "%d seconds", "%d milliseconds", "%d nanoseconds"};
    int sectionCount = sizeof(sections) / sizeof(int);

    int firstNonZero = sectionCount - 1;

    for(int i = 0; i < sectionCount; ++i) {
        if(sections[i] != 0) {
            firstNonZero = i;
            break;
        }
    }

    for(int i = firstNonZero; i < sectionCount; ++i) {
        printf(sectionNames[i], sections[i]);

        if(i != sectionCount - 1) {
            printf(", ");
        }
    }
    printf("\n");
}
