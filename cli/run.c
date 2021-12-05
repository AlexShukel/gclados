#include "run.h"

#include <errno.h>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builder.h"
#include "panic.h"
#include "testParser.h"

typedef struct {
    glob_t *paths;
    char **gccArgs;
    size_t gccArgCount;
} RunCommandOptions;

RunCommandOptions *parseRunArgs(int argc, char *argv[]) {
    RunCommandOptions *options = malloc(sizeof(RunCommandOptions));

    int doubleHyphenPosition = -1;

    for(int i = 0; i < argc; i++) {
        if(strcmp(argv[i], "--") == 0) {
            doubleHyphenPosition = i;
            break;
        }
    }

    if(doubleHyphenPosition != -1) {
        options->gccArgs = argv + doubleHyphenPosition + 1;
        options->gccArgCount = argc - doubleHyphenPosition - 1;
    } else {
        options->gccArgs = NULL;
        options->gccArgCount = 0;
    }

    if(argc > 0 && (doubleHyphenPosition == -1 || argc - doubleHyphenPosition > 0)) {
        glob_t *globBuffer = malloc(sizeof(glob_t));
        memset(globBuffer, 0, sizeof(glob_t));

        globBuffer->gl_pathc = 0;
        globBuffer->gl_pathv = NULL;
        globBuffer->gl_offs = 0;

        int queryCount = doubleHyphenPosition == -1 ? argc : doubleHyphenPosition;
        for(int i = 0; i < queryCount; i++) {
            if(glob(argv[i], GLOB_APPEND, NULL, globBuffer) != 0) {
                printf("Matching %s glob pattern failed: %s\n", argv[i], strerror(errno));
                return NULL;
            }
        }

        options->paths = globBuffer;
    } else {
        options->paths = NULL;
    }

    return options;
}

char *compileTestEntry(char *entryFilePath, RunCommandOptions options) {
    char commandBuffer[1024];

    size_t offset = sprintf(commandBuffer, "gcc %s", entryFilePath);

    for(int i = 0; i < options.paths->gl_pathc; i++) {
        offset += sprintf(commandBuffer + offset, " %s", options.paths->gl_pathv[i]);
    }

    for(int i = 0; i < options.gccArgCount; ++i) {
        offset += sprintf(commandBuffer + offset, " %s", options.gccArgs[i]);
    }

    char *outputFile = tmpNameExtended("");

    if(outputFile == NULL) {
        gcladosPanic("Could not create temporary file for test executable.", EXIT_FAILURE);
    }

    sprintf(commandBuffer + offset, " -o %s", outputFile);

    printf("Executing command \"%s\"...\n", commandBuffer);
    fflush(stdout);

    int exitCode = system(commandBuffer);

    if(exitCode) {
        gcladosPanic("Failed to compile tests entrypoint. Reason: gcc exited with non-zero exit code.", exitCode);
    }

    return outputFile;
}

int executeRun(RunCommandOptions *options) {
    if(options->paths != NULL && options->paths->gl_pathc > 0) {
        ParsedTestFile parsedFiles[options->paths->gl_pathc];

        for(size_t i = 0; i < options->paths->gl_pathc; ++i) {
            parsedFiles[i] = parseTestFile(options->paths->gl_pathv[i]);
        }

        // Generating temporary filename for output.
        char *filename = tmpNameExtended(".c");
        if(filename == NULL) {
            gcladosPanic("Could not create temporary file for tests entrypoint.", EXIT_FAILURE);
        }

        buildTestFile(filename, parsedFiles, options->paths->gl_pathc);

        char *compiled = compileTestEntry(filename, *options);

        int status = system(compiled);

        if(status != 0 && status != 256) {
            char buffer[100];

            sprintf(buffer, "Running tests failed with non-zero exit code. (%d)", status);
            gcladosPanic(buffer, status);
        }

        globfree(options->paths);
        free(filename);
        free(compiled);
    } else {
        printf("No tests found.\n");
    }

    return 0;
}

Command createRunCommand() {
    Command runCommand = {
            "run",
            "Command, which runs specified tests",
            (void *(*) (int, char *[])) parseRunArgs,
            (int(*)(void *)) executeRun,
    };

    return runCommand;
}
