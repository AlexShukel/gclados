#ifndef GCLADOS_COLORS_H
#define GCLADOS_COLORS_H

#include <stdbool.h>

typedef struct {
        int* flags;
        int count;
} GcladosAnsiFlags;

typedef enum {
    GCLADOS_BLACK = 0,
    GCLADOS_RED,
    GCLADOS_GREEN,
    GCLADOS_YELLOW,
    GCLADOS_BLUE,
    GCLADOS_MAGENTA,
    GCLADOS_CYAN,
    GCLADOS_WHITE,
} GcladosColor;

typedef struct {
    void (*setColorsSupported)(bool colorSupport);
    bool (*colorsSupported)();
    GcladosAnsiFlags (*createFlags)(int count, ...);
    char* (*applyFlags)(char* input, GcladosAnsiFlags);
    int (*foregroundColor)(GcladosColor color);
    int (*backgroundColor)(GcladosColor color);
    int (*bold)();
    int (*dimmed)();
    int (*italic)();
    int (*underline)();
    int (*invert)();
    int (*strikethrough)();
    int (*framed)();
} GcladosColors;

extern GcladosColors gcladosColors;

#endif