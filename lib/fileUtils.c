#include "fileUtils.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "panic.h"

void gcladosEnsureFileExists(const char filename[]) {
    FILE *file = fopen(filename, "r");

    bool exists = file != NULL;
    fclose(file);

    if(exists) {
        return;
    }

    FILE *newFile = fopen(filename, "w");

    if(newFile == NULL) {
        char buffer[FILENAME_MAX + 128];
        sprintf(buffer, "Could not open or create file \"%s\".", filename);
        gcladosPanic(buffer, EXIT_FAILURE);
    }

    fclose(newFile);
}

void gcladosMoveBlock(FILE *file, long n) {
    if(n == 0) {
        return;
    }

    long cursorPosition = ftell(file);
    long copyBeginning = n > 0 ? cursorPosition : cursorPosition - n;
    fseek(file, 0, SEEK_END);
    long fileLength = ftell(file);

    long copySize = fileLength - copyBeginning;
    void *memoryBlock = malloc(copySize);

    if(memoryBlock == NULL) {
        gcladosPanic("Could not move such big memory block.", EXIT_FAILURE);
    }

    fseek(file, copyBeginning, SEEK_SET);
    fread(memoryBlock, copySize, 1, file);
    fseek(file, copyBeginning + n, SEEK_SET);
    fwrite(memoryBlock, copySize, 1, file);

    free(memoryBlock);
}
