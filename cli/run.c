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
                return NULL;
            }
        }

        options->pathCount = globBuffer.gl_pathc;
        options->paths = globBuffer.gl_pathv;
    } else {
        return NULL;
    }

    return options;
}

int executeRun(struct RunCommandOptions* options) {
    printf("Running %ld tests...\n", options->pathCount);

    for(int i = 0; i < options->pathCount; i++) {
        printf("%s\n", options->paths[i]);
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
