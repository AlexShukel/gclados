// Author: Artiom Tretjakovas
// Description: This file contains function implementations from "filenameUtils.h".

#include "filenameUtils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *tmpNameExtended(char *extension) {
    size_t extensionLength = strlen(extension);
    // Allocating space for temporary filename.
    char *fileName = calloc(L_tmpnam + extensionLength, sizeof(char));

    // If generating temporary filename failed, return NULL.
    if(tmpnam(fileName) == NULL) {
        free(fileName);

        return NULL;
    }

    // Adding extension to the end of file.
    strcat(fileName, extension);

    return fileName;
}
