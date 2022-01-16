#include <assert.h>
#include <string.h>

#include "gclados.h"

void shouldCopyString() {
    gcladosColors.setColorsSupported(false);

    char *string = "Hello world";
    GcladosAnsiFlags flags = gcladosColors.createFlags(1, gcladosColors.bold());
    char *coloredString = gcladosColors.applyFlags(string, flags);

    assert(string != coloredString);

    free(coloredString);
    gcladosColors.setColorsSupported(true);
    coloredString = gcladosColors.applyFlags(string, flags);

    assert(string != coloredString);
    free(coloredString);

    gcladosColors.freeFlags(flags);
}

void shouldIdenticalCopyOfString() {
    // When colors are not supported
    gcladosColors.setColorsSupported(false);
    char *string = "Hello world";
    GcladosAnsiFlags flags =
            gcladosColors.createFlags(2, gcladosColors.bold(), gcladosColors.foregroundColor(GCLADOS_GREEN));
    char *coloredString = gcladosColors.applyFlags(string, flags);

    assert(strcmp(string, coloredString) == 0);

    free(coloredString);
    gcladosColors.freeFlags(flags);

    // As well as there is no flags
    gcladosColors.setColorsSupported(true);
    flags = gcladosColors.createFlags(0);
    coloredString = gcladosColors.applyFlags(string, flags);

    assert(strcmp(string, coloredString) == 0);
    free(coloredString);
    gcladosColors.freeFlags(flags);
}

void shouldReturnColoredString() {
    gcladosColors.setColorsSupported(true);
    char *string = "Hello world";
    GcladosAnsiFlags flags =
            gcladosColors.createFlags(2, gcladosColors.bold(), gcladosColors.foregroundColor(GCLADOS_GREEN));
    char *coloredString = gcladosColors.applyFlags(string, flags);

    assert(strcmp("\x1b[01;32mHello world\x1b[0m", coloredString) == 0);

    free(coloredString);
    gcladosColors.freeFlags(flags);
}

void runColorTests() {
    shouldCopyString();
    shouldIdenticalCopyOfString();
    shouldReturnColoredString();
}