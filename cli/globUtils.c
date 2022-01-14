#include "globUtils.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char EMATCH_GLOB[] = "Matching \"%s\" glob pattern failed: %s\n";

glob_t *matchAll(const char *patterns[], size_t patternCount) {
    glob_t *globBuffer = malloc(sizeof(glob_t));
    memset(globBuffer, 0, sizeof(glob_t));

    for(int i = 0; i < patternCount; i++) {
        if(glob(patterns[i], GLOB_APPEND, NULL, globBuffer) != 0) {
            fprintf(stderr, EMATCH_GLOB, patterns[i], strerror(errno));
            free(globBuffer);

            return NULL;
        }
    }

    return globBuffer;
}
