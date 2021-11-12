#include "filenameUtils.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

char* tmpNameExtended(char* extension) {
    size_t extensionLength = strlen(extension);
    char* fileName = calloc(L_tmpnam + extensionLength, sizeof(char));

    if(tmpnam(fileName) == NULL) {
        free(fileName);

        return NULL;
    }

    size_t fileNameLength = strlen(fileName);
    for(int i = 0; i < extensionLength; i++) {
        fileName[fileNameLength + i] = extension[i];
    }

    fileName[fileNameLength + extensionLength] = '\0';

    return fileName;
}
