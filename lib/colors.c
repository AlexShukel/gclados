#include "colors.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

bool PTF_SUPPORTED_COLORS = true;

// ANSI codes taken from https://en.wikipedia.org/wiki/ANSI_escape_code#SGR_(Select_Graphic_Rendition)_parameters
const int PTF_FOREGROUND_COLOR_OFFSET = 30;
const int PTF_BACKGROUND_COLOR_OFFSET = 40;
const int PTF_RESET = 0;
const int PTF_BOLD = 1;
const int PTF_DIMMED = 2;
const int PTF_ITALIC = 3;
const int PTF_UNDERLINE = 4;
const int PTF_INVERT = 7;
const int PTF_STRIKETHROUGH = 9;
const int PTF_FRAMED = 51;

void ptfSetColorsSupported(bool supported) {
    PTF_SUPPORTED_COLORS = supported;
}

bool ptfColorsSupported() {
    return PTF_SUPPORTED_COLORS;
}

char* ptfApplyAnsiFlags(char* string, struct PtfAnsiFlags flags) {
    if(!ptfColorsSupported()) {
        return string;
    }

    char* newString = calloc(strlen(string) + flags.count * 3 + 6, sizeof(char));

    int currentOffset = sprintf(newString, "\x1b[");

    for(int i = 0; i < flags.count; i++) {
        currentOffset += sprintf(newString + currentOffset, "%02d%c", flags.flags[i], i != flags.count - 1 ? ';' : 'm');
    }

    sprintf(newString + currentOffset, "%s\x1b[%dm", string, PTF_RESET);

    return newString;
}

struct PtfAnsiFlags ptfCreateAnsiFlags(int count, ...) {
    int *flags = calloc(count, sizeof(int));

    va_list list;
    va_start(list, count);

    for(int i = 0; i < count; i++) {
        flags[i] = va_arg(list, int);
    }

    va_end(list);

    struct PtfAnsiFlags output = {
            .flags = flags,
            .count = count
    };

    return output;
}

int ptfForegroundColor(enum PtfColor color) {
    return (int) (PTF_FOREGROUND_COLOR_OFFSET + color);
}

int ptfBackgroundColor(enum PtfColor color) {
    return (int)(PTF_BACKGROUND_COLOR_OFFSET + color);
}

int ptfBold() {
    return PTF_BOLD;
}

int ptfDimmed() {
    return PTF_DIMMED;
}

int ptfItalic() {
    return PTF_ITALIC;
}

int ptfUnderline() {
    return PTF_UNDERLINE;
};

int ptfInvert() {
    return PTF_INVERT;
}

int ptfStrikethrough() {
    return PTF_STRIKETHROUGH;
}

int ptfFramed() {
    return PTF_FRAMED;
}

struct PtfColors ptfColors = {
        .setColorsSupported = ptfSetColorsSupported,
        .colorsSupported = ptfColorsSupported,
        .createFlags = ptfCreateAnsiFlags,
        .applyFlags = ptfApplyAnsiFlags,
        .foregroundColor = ptfForegroundColor,
        .backgroundColor = ptfBackgroundColor,
        .bold = ptfBold,
        .dimmed = ptfDimmed,
        .italic = ptfItalic,
        .underline = ptfUnderline,
        .invert = ptfInvert,
        .strikethrough = ptfStrikethrough,
        .framed = ptfFramed,
};