#include "ioutils.h"
#include "stdlib.h"
#include "stdio.h"
#include "panic.h"

void ptfPrintLineNumber(int number, bool highlight) {
    printf("\n%c %3d | ", highlight ? '>' : ' ', number);
}

void ptfPrintFileLines(FILE* file, int lineBegin, int lineEnd, int highlightedLine) {
    if(lineBegin < 1 || lineEnd < 1) {
        ptfPanic("Begin and end lines should be not less than 1");
    }

    int currentCharacter, currentLine = 1;

    while(currentCharacter = fgetc(file),
            currentLine += (currentCharacter == '\n'),
            currentLine < lineBegin && currentCharacter != EOF)
        ;

    ptfPrintLineNumber(currentLine, currentLine == highlightedLine);

    while(currentCharacter = fgetc(file), currentLine <= lineEnd && currentCharacter != EOF) {
        if(currentCharacter == '\n') {
            if(++currentLine <= lineEnd) {
                ptfPrintLineNumber(currentLine, currentLine == highlightedLine);
            }
        } else if(currentCharacter == '\r') {
            continue;
        } else {
            putc(currentCharacter, stdout);
        }
    }

    putc('\n', stdout);

    if(currentLine < lineEnd) {
        ptfPanic("Incomplete file segment was printed - end of file was reached"
                 " or an unexpected error occurred.");
    }
}

void ptfPrintProgress(FILE* file, double percentage, size_t width) {
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

char* ptfStandardErrorMessage(bool pass, char* usage, char* expected, char* received) {
    const struct PtfAnsiFlags expectedValueFlags = ptfColors.createFlags(2,
            ptfColors.foregroundColor(PTF_GREEN),
            ptfColors.bold());
    const struct PtfAnsiFlags receivedValueFlags = ptfColors.createFlags(2,
            ptfColors.foregroundColor(PTF_RED),
            ptfColors.bold());

    char* messageBuff = calloc(1024, sizeof(char));
    int offset = 0;

    if(usage != NULL) {
        char* expectedString = ptfColors.applyFlags("expected", expectedValueFlags);
        char* receivedString = ptfColors.applyFlags("received", receivedValueFlags);

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
        char* expectedBuffColorized = ptfColors.applyFlags(expectedBuff, receivedValueFlags);
        offset += sprintf(messageBuff + offset, "    Expected: %s", expectedBuffColorized);

        free(expectedBuff);
        free(expectedBuffColorized);
    } else {
        char* expectedColorized = ptfColors.applyFlags(expected, expectedValueFlags);
        char* receivedColorized = ptfColors.applyFlags(received, receivedValueFlags);

        offset += sprintf(messageBuff + offset, "    Expected: %s\n    Received: %s", expectedColorized, receivedColorized);

        free(expectedColorized);
        free(receivedColorized);
    }

    return messageBuff;
}
