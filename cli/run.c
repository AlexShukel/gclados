#include "run.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glob.h>
#include "testParser.h"
#include "builder.h"

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
        struct ParsedTest tests[100];
        size_t count = parseTestFile(options->paths[0], tests);

        char* testFile = buildTestFile(tests, count);

        FILE* file = fopen(testFile, "r");

        char buff[1024];

        int c = fread(buff, sizeof(char), 1024, file);

        printf("%s", buff);

        fclose(file);

        char command[1024];
        sprintf(command, "gcc -c -x c %s -I./../../lib -o C:/Users/artio/CLionProjects/propag-testing-framework/cmake-build-debug/hello.o", testFile);

        system(command);
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
