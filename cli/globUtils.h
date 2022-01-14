#ifndef GCLADOS_GLOB_UTILS_H
#define GCLADOS_GLOB_UTILS_H

#include <glob.h>

// Run glob for all specified patterns.
glob_t *matchAll(const char *patterns[], size_t patternCount);

#endif
