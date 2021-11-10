#ifndef __PTF_COLORS_H__
#define __PTF_COLORS_H__

#include <stdbool.h>

struct PtfAnsiFlags {
        int* flags;
        int count;
};

enum PtfColor {
    PTF_BLACK = 0,
    PTF_RED,
    PTF_GREEN,
    PTF_YELLOW,
    PTF_BLUE,
    PTF_MAGENTA,
    PTF_CYAN,
    PTF_WHITE,
};

struct PtfColors {
    void (*setColorsSupported)(bool colorSupport);
    bool (*colorsSupported)();
    struct PtfAnsiFlags (*createFlags)(int count, ...);
    char* (*applyFlags)(char* input, struct PtfAnsiFlags);
    int (*foregroundColor)(enum PtfColor color);
    int (*backgroundColor)(enum PtfColor color);
};

extern struct PtfColors ptfColors;

#endif