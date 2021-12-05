// Author: Artiom Tretjakovas
// Description: This file contains implementation of "gclados help" command. Function prototypes is in "run.h" file.

#include "run.h"

#include <errno.h>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builder.h"
#include "panic.h"
#include "testParser.h"

// This structure contains options, that are required for the run command.
typedef struct {
    // Paths to the tests.
    glob_t *paths;
    // Custom GCC compiler arguments.
    char **gccArgs;
    // Count of "gccArgs" array.
    size_t gccArgCount;
} RunCommandOptions;

// Function, that parses arguments for the run command.
RunCommandOptions *parseRunArgs(int argc, char *argv[]) {
    // Allocating space for the options.
    RunCommandOptions *options = malloc(sizeof(RunCommandOptions));

    // Finding where is the double hyphen (--).
    // This is required because after double hyphen user can specify custom gcc arguments.
    int doubleHyphenPosition = -1;

    for(int i = 0; i < argc; i++) {
        if(strcmp(argv[i], "--") == 0) {
            doubleHyphenPosition = i;
            break;
        }
    }

    // If double hyphen is found, setting up the custom GCC args.
    if(doubleHyphenPosition != -1) {
        options->gccArgs = argv + doubleHyphenPosition + 1;
        options->gccArgCount = argc - doubleHyphenPosition - 1;
    } else {
        options->gccArgs = NULL;
        options->gccArgCount = 0;
    }

    if(argc > 0 && (doubleHyphenPosition == -1 || argc - doubleHyphenPosition > 0)) {
        // Finding all paths, that are specified as glob patterns.
        glob_t *globBuffer = malloc(sizeof(glob_t));
        memset(globBuffer, 0, sizeof(glob_t));

        globBuffer->gl_pathc = 0;
        globBuffer->gl_pathv = NULL;
        globBuffer->gl_offs = 0;

        // Calculating the glob pattern count. All arguments, that are not parsed and which are before the double hyphen
        // will be treated as glob patterns.
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

// Function, that compiles test entry.
// Parameters:
//     *entryFilePath - full path to the entry file, that was built using "buildTestFile" command.
//     options        - all options of the run command.
// Returns:
//     The path to the executable, or NULL if compilation failed.
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

// Function, that executes "run" command.
int executeRun(RunCommandOptions *options) {
    if(options->paths != NULL && options->paths->gl_pathc > 0) {
        printf("No tests found.\n");

        return EXIT_FAILURE;
    }

    ParsedTestFile parsedFiles[options->paths->gl_pathc];

    // Parsing all files.
    for(size_t i = 0; i < options->paths->gl_pathc; ++i) {
        parsedFiles[i] = parseTestFile(options->paths->gl_pathv[i]);
    }

    // Generating temporary filename for output.
    char *filename = tmpNameExtended(".c");
    if(filename == NULL) {
        gcladosPanic("Could not create temporary file for tests entrypoint.", EXIT_FAILURE);
    }

    int buildStatusCode = buildTestFile(filename, parsedFiles, options->paths->gl_pathc);

    if(buildStatusCode) {
        gcladosPanic("Failed to build tests entrypoint.", buildStatusCode);
    }

    char *compiled = compileTestEntry(filename, *options);

    // Running compiled executable.
    int status = system(compiled);

    // Status 0 - success, 256 - tests failed.
    // Status 256 should not be treated as command failure, because this means that tests were compiled & run
    //   successfully, but not passed.
    if(status != 0 && status != 256) {
        char buffer[100];

        sprintf(buffer, "Running tests failed with non-zero exit code. (%d)", status);
        gcladosPanic(buffer, status);
    }

    globfree(options->paths);
    free(filename);
    free(compiled);

    return EXIT_SUCCESS;
}

Command createRunCommand() {
    Command runCommand = {
            "run",
            "Command, which runs specified tests",
            (void *(*) (const int, const char **) ) parseRunArgs,
            (const int (*)(const void *)) executeRun,
    };

    return runCommand;
}
