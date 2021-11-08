#include "run.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct RunCommandOptions {
    char* testFile;
};

struct RunCommandOptions* parseRunArgs(int argc, char* argv[]) {
    struct RunCommandOptions* options = malloc(sizeof(struct RunCommandOptions));

    if(argc > 0) {
        options->testFile = argv[0];
    } else {
        return NULL;
    }

    return options;
}

int executeRun(struct RunCommandOptions* options) {
    printf("Running tests from %s\n", options->testFile);

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
