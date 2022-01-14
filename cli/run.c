// Author: Artiom Tretjakovas
// Description: This file contains implementation of "gclados help" command. Function prototypes is in "run.h" file.

#include "run.h"

#include <errno.h>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builder.h"
#include "globUtils.h"
#include "panic.h"

// Command specific strings
const char RUN_COMMAND_SLUG[] = "run";
const char RUN_COMMAND_SHORT_HELP[] = "Execute tests by glob pattern.";
const char RUN_COMMAND_HELP[] =
        "Compiles & runs tests, which match specified glob pattern.\n"
        "All options specified after command name and before double hyphen (--) will be treated as glob patterns.\n"
        "All options after double hyphen (--) will be passed directly into GCC compiler.\n"
        "If you would like to use other compiler than GCC, you can use \"generate\" command. See (gclados help "
        "generate)\n"
        "Examples:\n"
        "Let's assume your project has this structure:\n"
        "  project\n"
        "  |- README.txt\n"
        "  |- src\n"
        "  |  |- main.c\n"
        "  |  |- lib.c\n"
        "  |  |- lib.test.c\n"
        "  |- tests\n"
        "     |- main.test.c\n"
        "     |- another.test.c\n"
        "Glob pattern examples.\n"
        "  If you would like to run all files, which have \".test.c\" extension, you can run:\n"
        "    gclados run *.test.c\n"
        "  This will match \"lib.test.c\", \"main.test.c\" and \"another.test.c\" from the structure above.\n"
        "  If you would like to run only tests which are in \"tests\" directory, you can run:\n"
        "    gclados run ./tests/*.test.c\n"
        "  This will match \"main.test.c\" and \"another.test.c\" from the structure above.\n"
        "  It is not necessary to specify only one glob pattern. For example, you can specify two globs:\n"
        "    gclados run ./tests/*.test.c ./src/*.test.c\n"
        "  This will match \"lib.test.c\", \"main.test.c\" and \"another.test.c\" from the structure above.\n"
        "  Also, you can specify just direct path to the tests, not glob pattern:\n"
        "    gclados run ./src/lib.test.c\n"
        "  This will match only \"lib.test.c\" from the structure above.\n"
        "GCC arguments.\n"
        "  GcLaDOS cannot automatically determine, which files should be included into executable.\n"
        "  By default, GcLaDOS includes only files, which were specified via glob patterns.\n"
        "  A workaround for this issue is to specify additional GCC arguments after double hyphen.\n"
        "  For example:\n"
        "    gclados run *.test.c -- ./src/main.c ./src/lib.c\n"
        "  This command will include \"main.c\" and \"lib.c\" into executable.";

// Exceptions
const char EMATCHING_GLOB_PATTERN[] = "Matching \"%s\" glob pattern failed: %s.\n";
const char ECREATE_EXECUTABLE_FAILED[] = "Could not create temporary file for test executable.";
const char ECOMPILATION_FAILED[] = "Failed to compile tests entrypoint. Reason: gcc exited with non-zero exit code.";
const char ENOT_FOUND[] = "No tests found.\n";
const char ECREATE_ENTRY_FAILED[] = "Could not create temporary file for tests entrypoint.";
const char EBUILD_ENTRY_FAILED[] = "Failed to build tests entrypoint.";
const char ETEST_RUN_FAILED[] = "Running tests failed with non-zero exit code. (%d)";

// Info messages
const char EXECUTING_COMMAND[] = "Executing command \"%s\"...\n";

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
        // Calculating the glob pattern count. All arguments, that are not parsed and which are before the double hyphen
        // will be treated as glob patterns.
        size_t queryCount = doubleHyphenPosition == -1 ? argc : doubleHyphenPosition;

        options->paths = matchAll((const char **) argv, queryCount);
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
        gcladosPanic(ECREATE_EXECUTABLE_FAILED, EXIT_FAILURE);
    }

    sprintf(commandBuffer + offset, " -o %s", outputFile);

    printf(EXECUTING_COMMAND, commandBuffer);
    fflush(stdout);

    int exitCode = system(commandBuffer);

    if(exitCode) {
        gcladosPanic(ECOMPILATION_FAILED, exitCode);
    }

    return outputFile;
}

// Function, that executes "run" command.
int executeRun(RunCommandOptions *options) {
    if(options->paths == NULL || options->paths->gl_pathc == 0) {
        fprintf(stderr, ENOT_FOUND);

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
        gcladosPanic(ECREATE_ENTRY_FAILED, EXIT_FAILURE);
    }

    FILE *entry = fopen(filename, "w");

    if(entry == NULL) {
        gcladosPanic(ECREATE_ENTRY_FAILED, EXIT_FAILURE);
    }

    int buildStatusCode = buildTestFile(entry, parsedFiles, options->paths->gl_pathc);

    fclose(entry);

    if(buildStatusCode) {
        gcladosPanic(EBUILD_ENTRY_FAILED, buildStatusCode);
    }

    char *compiled = compileTestEntry(filename, *options);

    // Running compiled executable.
    int status = system(compiled);

    // Status 0 - success, 256 - tests failed.
    // Status 256 should not be treated as panic, because this means that tests were compiled & run
    //   successfully, but not passed.
    if(status != 0 && status != 256) {
        char buffer[100];

        sprintf(buffer, ETEST_RUN_FAILED, status);
        gcladosPanic(buffer, status);
    }

    globfree(options->paths);
    free(filename);
    free(compiled);

    return EXIT_SUCCESS;
}

Command createRunCommand() {
    Command runCommand = {
            .slug = RUN_COMMAND_SLUG,
            .shortHelp = RUN_COMMAND_SHORT_HELP,
            .help = RUN_COMMAND_HELP,
            .parseArgs = (void *(*) (const int, const char **) ) parseRunArgs,
            .execute = (const int (*)(const void *)) executeRun,
    };

    return runCommand;
}
