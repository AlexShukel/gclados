#ifndef GCLADOS_IO_UTILS_H
#define GCLADOS_IO_UTILS_H

#include <stdbool.h>
#include <stdio.h>
#include <test.h>

char *gcladosGetFailedStatementMessage(bool pass, GcladosPredicate predicate, void *value);
char *gcladosStandardErrorMessage(bool pass, char *usage, char *expected, char *received);
void gcladosPrintProgress(FILE *file, double percentage, size_t width);
void gcladosPrintFileLines(FILE *file, int lineBegin, int lineEnd, int highlightedLine);
char *gcladosConvertToHex(void *ptr, size_t size);

#endif
