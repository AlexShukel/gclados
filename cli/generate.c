// Author: Artiom Tretjakovas
// Description: This file contains implementation of "gclados generate" command. Function prototype is in "generate.h"
//              file.

#include "generate.h"

#include <errno.h>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"
#include "builder.h"
#include "testParser.h"

// Structure for saving options for the generate command.
typedef struct {
    // Variable, that contains paths to the specified tests.
    glob_t *paths;
    // Output file path.
    char *outputFile;
} GenerateCommandOptions;

// Function, which parses generate command's arguments
GenerateCommandOptions *parseGenerateArgs(int argc, char *argv[]) {
    // Allocating space for options.
    GenerateCommandOptions *options = malloc(sizeof(GenerateCommandOptions));

    Argument arguments[] = {createStringArgument("output", "Specify output file.")};

    // Parsing arguments.
    void **parsedArguments = parseArguments(arguments, sizeof(arguments) / sizeof(Argument), &argc, argv);

    char *outputFile = (char *) parsedArguments[0];
    options->outputFile = outputFile;

    free(parsedArguments);

    if(argc > 0) {
        // Running glob for all specified paths.

        glob_t *globBuffer = malloc(sizeof(glob_t));
        memset(globBuffer, 0, sizeof(glob_t));

        globBuffer->gl_pathc = 0;
        globBuffer->gl_pathv = NULL;
        globBuffer->gl_offs = 0;

        for(int i = 0; i < argc; i++) {
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

// Function, which executes generate command.
int executeGenerate(const GenerateCommandOptions *options) {
    // Failure if output file is NULL.
    if(options->outputFile == NULL) {
        if(options->paths != NULL) {
            globfree(options->paths);
        }
        free((GenerateCommandOptions *) options);

        printf("Not output file specified.\n");
        return 1;
    }

    // Failure if no tests specified.
    if(options->paths == NULL || options->paths->gl_pathc == 0) {
        printf("No tests found.\n");

        if(options->paths != NULL) {
            globfree(options->paths);
        }
        free(options->outputFile);
        free((GenerateCommandOptions *) options);

        return 1;
    }

    // Parsing test files.
    ParsedTestFile parsedFiles[options->paths->gl_pathc];

    for(size_t i = 0; i < options->paths->gl_pathc; ++i) {
        parsedFiles[i] = parseTestFile(options->paths->gl_pathv[i]);
    }

    // Building test entry.
    int buildStatus = buildTestFile(options->outputFile, parsedFiles, options->paths->gl_pathc);

    if(buildStatus) {
        printf("Generation failure.\n");
    } else {
        printf("Generation succeeded: output written to \"%s\".\n", options->outputFile);
    }

    // Cleanups.
    globfree(options->paths);
    free(options->outputFile);
    free((GenerateCommandOptions *) options);

    return buildStatus;
}

Command createGenerateCommand() {
    Command runCommand = {
            "generate",
            "Command, which generates test entry from specified files",
            (void *(*) (const int, const char **) ) parseGenerateArgs,
            (const int (*)(const void *)) executeGenerate,
    };

    return runCommand;
}
