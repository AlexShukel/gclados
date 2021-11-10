#include "colors.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

bool PTF_SUPPORTED_COLORS = true;

void ptfSetColorsSupported(bool supported) {
    PTF_SUPPORTED_COLORS = false;
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

    sprintf(newString + currentOffset, "%s\x1b[0m", string);

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
    return 30 + color;
}

int ptfBackgroundColor(enum PtfColor color) {
    return 40 + color;
}

struct PtfColors ptfColors = {
        .setColorsSupported = ptfSetColorsSupported,
        .colorsSupported = ptfColorsSupported,
        .createFlags = ptfCreateAnsiFlags,
        .applyFlags = ptfApplyAnsiFlags,
        .foregroundColor = ptfForegroundColor,
        .backgroundColor = ptfBackgroundColor,
};