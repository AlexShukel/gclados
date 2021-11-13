#ifndef __PTF_IO_UTILS_H__
#define __PTF_IO_UTILS_H__

#include <stdbool.h>
#include <stdio.h>
#include "colors.h"

char* ptfStandardErrorMessage(bool pass, char* usage, char* expected, char* received);
void ptfPrintProgress(FILE* file, double percentage, size_t width);
void ptfPrintFileLines(FILE* file, int lineBegin, int lineEnd, int highlightedLine);

#endif
