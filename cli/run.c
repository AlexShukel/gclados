#include "run.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glob.h>

struct RunCommandOptions {
    char** paths;
    size_t pathCount;
};

int printError(const char* message, int a) {
    return printf("%s (%d)", message, a);
}

struct RunCommandOptions* parseRunArgs(int argc, char* argv[]) {
    struct RunCommandOptions* options = malloc(sizeof(struct RunCommandOptions));

    if(argc > 0) {
        glob_t globBuffer;

        globBuffer.gl_pathc = 0;
        globBuffer.gl_pathv = NULL;
        globBuffer.gl_offs = 0;

        for(int i = 0; i < argc; i++) {
            if(glob(argv[i], GLOB_APPEND, printError, &globBuffer) != 0) {
                printf("Error!\n");
                return NULL;
            }
        }

        options->pathCount = globBuffer.gl_pathc;
        options->paths = globBuffer.gl_pathv;
    } else {
        printf("Failed to parse arguments.\n");
        return NULL;
    }

    return options;
}

int executeRun(struct RunCommandOptions* options) {
    printf("Running %ld tests...\n", options->pathCount);

    if(options->pathCount > 0) {
        readTestFile(options->paths[0]);
    }

    return 0;
}

struct Command createRunCommand() {
    struct Command runCommand = {
            "run",
            "Command, which runs specified tests",
            (void* (*)(int, char*[])) parseRunArgs,
            (int (*)(void*)) executeRun,
    };

    return runCommand;
}
