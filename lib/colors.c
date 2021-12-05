// Author: Artiom Tretjakovas
// Description: This file contains implementation of text coloring functions. Function prototypes are described in
//              "colors.h" file.

#include "colors.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Variable, that saves color support.
bool GCLADOS_SUPPORTED_COLORS = true;

// ANSI codes taken from https://en.wikipedia.org/wiki/ANSI_escape_code#SGR_(Select_Graphic_Rendition)_parameters
const int GCLADOS_FOREGROUND_COLOR_OFFSET = 30;
const int GCLADOS_BACKGROUND_COLOR_OFFSET = 40;
const int GCLADOS_RESET = 0;
const int GCLADOS_BOLD = 1;
const int GCLADOS_DIMMED = 2;
const int GCLADOS_ITALIC = 3;
const int GCLADOS_UNDERLINE = 4;
const int GCLADOS_INVERT = 7;
const int GCLADOS_STRIKETHROUGH = 9;
const int GCLADOS_FRAMED = 51;

void gcladosSetColorsSupported(bool supported) {
    GCLADOS_SUPPORTED_COLORS = supported;
}

bool gcladosColorsSupported() {
    return GCLADOS_SUPPORTED_COLORS;
}

char *gcladosApplyAnsiFlags(char *string, GcladosAnsiFlags flags) {
    if(!gcladosColorsSupported()) {
        char *newString = calloc(strlen(string), sizeof(char));
        sprintf(newString, "%s", string);
        return newString;
    }

    char *newString = calloc(strlen(string) + flags.count * 3 + 6, sizeof(char));

    // ANSI opening tag.
    int currentOffset = sprintf(newString, "\x1b[");

    for(int i = 0; i < flags.count; i++) {
        currentOffset += sprintf(newString + currentOffset, "%02d%c", flags.flags[i], i != flags.count - 1 ? ';' : 'm');
    }

    // ANSI closing tag.
    sprintf(newString + currentOffset, "%s\x1b[%dm", string, GCLADOS_RESET);

    return newString;
}

GcladosAnsiFlags gcladosCreateAnsiFlags(int count, ...) {
    int *flags = calloc(count, sizeof(int));

    va_list list;
    va_start(list, count);

    for(int i = 0; i < count; i++) {
        flags[i] = va_arg(list, int);
    }

    va_end(list);

    GcladosAnsiFlags output = {.flags = flags, .count = count};

    return output;
}

void gcladosFreeFlags(GcladosAnsiFlags flags) {
    free(flags.flags);
}

int gcladosForegroundColor(GcladosColor color) {
    return (int) (GCLADOS_FOREGROUND_COLOR_OFFSET + color);
}

int gcladosBackgroundColor(GcladosColor color) {
    return (int) (GCLADOS_BACKGROUND_COLOR_OFFSET + color);
}

int gcladosBold() {
    return GCLADOS_BOLD;
}

int gcladosDimmed() {
    return GCLADOS_DIMMED;
}

int gcladosItalic() {
    return GCLADOS_ITALIC;
}

int gcladosUnderline() {
    return GCLADOS_UNDERLINE;
}

int gcladosInvert() {
    return GCLADOS_INVERT;
}

int gcladosStrikethrough() {
    return GCLADOS_STRIKETHROUGH;
}

int gcladosFramed() {
    return GCLADOS_FRAMED;
}

GcladosColors gcladosColors = {
        .setColorsSupported = gcladosSetColorsSupported,
        .colorsSupported = gcladosColorsSupported,
        .createFlags = gcladosCreateAnsiFlags,
        .freeFlags = gcladosFreeFlags,
        .applyFlags = gcladosApplyAnsiFlags,
        .foregroundColor = gcladosForegroundColor,
        .backgroundColor = gcladosBackgroundColor,
        .bold = gcladosBold,
        .dimmed = gcladosDimmed,
        .italic = gcladosItalic,
        .underline = gcladosUnderline,
        .invert = gcladosInvert,
        .strikethrough = gcladosStrikethrough,
        .framed = gcladosFramed,
};