#include "generate.h"

#include <errno.h>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"
#include "builder.h"
#include "testParser.h"

typedef struct {
    glob_t *paths;
    char *outputFile;
} GenerateCommandOptions;

GenerateCommandOptions *parseGenerateArgs(int argc, char *argv[]) {
    GenerateCommandOptions *options = malloc(sizeof(GenerateCommandOptions));

    Argument arguments[] = {createStringArgument("output", "Specify output file.")};

    void **parsedArguments = parseArguments(arguments, sizeof(arguments) / sizeof(Argument), &argc, argv);

    char *outputFile = (char *) parsedArguments[0];
    options->outputFile = outputFile;

    if(argc > 0) {
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

int executeGenerate(const GenerateCommandOptions *options) {
    if(options->outputFile == NULL) {
        printf("Not output file specified.\n");
        return 1;
    }

    if(options->paths == NULL || options->paths->gl_pathc == 0) {
        printf("No tests found.\n");
        return 1;
    }

    ParsedTestFile parsedFiles[options->paths->gl_pathc];

    for(size_t i = 0; i < options->paths->gl_pathc; ++i) {
        parsedFiles[i] = parseTestFile(options->paths->gl_pathv[i]);
    }

    int buildStatus = buildTestFile(options->outputFile, parsedFiles, options->paths->gl_pathc);

    if(buildStatus) {
        printf("Generation failure.\n");
    } else {
        printf("Generation succeeded: output written to \"%s\".\n", options->outputFile);
    }

    globfree(options->paths);
    free(options->outputFile);

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
