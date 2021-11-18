#include "ioutils.h"

#include <stdlib.h>
#include <stdio.h>

#include "panic.h"
#include "colors.h"

void gcladosPrintLineNumber(int number, bool highlight) {
    printf("\n%c %3d | ", highlight ? '>' : ' ', number);
}

void gcladosPrintFileLines(FILE* file, int lineBegin, int lineEnd, int highlightedLine) {
    if(lineBegin < 1 || lineEnd < 1) {
        gcladosPanic("Begin and end lines should be not less than 1");
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

    if(currentLine < lineEnd) {
        gcladosPanic("Incomplete file segment was printed - end of file was reached"
                 " or an unexpected error occurred.");
    }
}

void gcladosPrintProgress(FILE* file, double percentage, size_t width) {
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

char* gcladosStandardErrorMessage(bool pass, char* usage, char* expected, char* received) {
    const GcladosAnsiFlags expectedValueFlags =
            gcladosColors.createFlags(2,
                                      gcladosColors.foregroundColor(GCLADOS_GREEN),
                                      gcladosColors.bold());
    const GcladosAnsiFlags receivedValueFlags =
            gcladosColors.createFlags(2,
                                      gcladosColors.foregroundColor(GCLADOS_RED),
                                      gcladosColors.bold());

    char* messageBuff = calloc(1024, sizeof(char));
    int offset = 0;

    if(usage != NULL) {
        char* expectedString = gcladosColors.applyFlags("expected", expectedValueFlags);
        char* receivedString = gcladosColors.applyFlags("received", receivedValueFlags);

        char* usageColorized = calloc(256, sizeof(char));

        sprintf(usageColorized, usage, receivedString);

        offset += sprintf(messageBuff, "  ensure(%s, %s);\n\n", expectedString, usageColorized);

        free(expectedString);
        free(receivedString);
        free(usageColorized);
    }

    if(pass) {
        char* expectedBuff = calloc(256, sizeof(char));
        sprintf(expectedBuff, "not %s", expected);
        char* expectedBuffColorized = gcladosColors.applyFlags(expectedBuff, receivedValueFlags);
        offset += sprintf(messageBuff + offset, "    Expected: %s", expectedBuffColorized);

        free(expectedBuff);
        free(expectedBuffColorized);
    } else {
        char* expectedColorized = gcladosColors.applyFlags(expected, expectedValueFlags);
        char* receivedColorized = gcladosColors.applyFlags(received, receivedValueFlags);

        offset += sprintf(messageBuff + offset, "    Expected: %s\n    Received: %s", expectedColorized, receivedColorized);

        free(expectedColorized);
        free(receivedColorized);
    }

    return messageBuff;
}
