// Author: Artiom Tretjakovas
// Description: This file contains implementation of "gclados generate" command. Function prototype is in "generate.h"
//              file.

#include "generate.h"

#include <errno.h>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>

#include "args.h"
#include "builder.h"
#include "gclados.h"
#include "globUtils.h"

// Command specific strings
const char GEN_COMMAND_SLUG[] = "generate";
const char GEN_COMMAND_SHORT_HELP[] = "Generate entrypoint for specified tests.";
const char GEN_COMMAND_HELP[] =
        "Generate entrypoint for tests, that match specified glob patterns.\n"
        "Usually, this command is used when it is necessary to use other compiler (not GCC).\n"
        "Options:\n"
        "  --output - Specify path to the output file. If not specified, output will appear in stdout.\n"
        "  --colors - Enables / disables colored mode. Recommended for terminals, which do not support them.\n"
        "   ...rest - The rest of the parameters will be treated as glob patterns.\n";

// Exceptions
const char ECREATE_OUTPUT_FAILED[] = "Failed creating output file.\n";
const char EGEN_FAILED[] = "Generation failure.\n";
const char EGEN_NO_TESTS[] = "No tests found.\n";

// Info messages
const char GEN_SUCCESS_STDOUT[] = "Generation success.\n";
const char GEN_SUCCESS_OUTFILE[] = "Generation succeeded, output written to \"%s\".\n";

// Options
const char OUTPUT_OPTION_SLUG[] = "output";

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

    Argument arguments[] = {
            createStringArgument(OUTPUT_OPTION_SLUG, NULL),
    };

    // Parsing arguments.
    void **parsedArguments = parseArguments(arguments, sizeof(arguments) / sizeof(Argument), &argc, argv);

    char *outputFile = (char *) parsedArguments[0];
    options->outputFile = outputFile;

    free(parsedArguments);

    if(argc > 0) {
        options->paths = matchAll((const char **) argv, argc);
    } else {
        options->paths = NULL;
    }

    return options;
}

// Function, which executes generate command.
int executeGenerate(const GenerateCommandOptions *options) {
    // Failure if no tests specified.
    if(options->paths == NULL || options->paths->gl_pathc == 0) {
        fprintf(stderr, EGEN_NO_TESTS);

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

    FILE *outputFile = options->outputFile == NULL ? stdout : fopen(options->outputFile, "w");

    if(outputFile == NULL) {
        gcladosPanic(ECREATE_OUTPUT_FAILED, EXIT_FAILURE);
    }

    // Building test entry.
    int buildStatus = buildTestFile(outputFile, parsedFiles, options->paths->gl_pathc);

    if(options->outputFile != NULL) {
        fclose(outputFile);
    }

    if(buildStatus) {
        fprintf(stderr, EGEN_FAILED);
    } else {
        printf(options->outputFile == NULL ? GEN_SUCCESS_STDOUT : GEN_SUCCESS_OUTFILE, options->outputFile);
    }

    // Cleanups.
    globfree(options->paths);
    free(options->outputFile);
    free((GenerateCommandOptions *) options);

    return buildStatus;
}

Command createGenerateCommand() {
    Command runCommand = {
            .slug = GEN_COMMAND_SLUG,
            .shortHelp = GEN_COMMAND_SHORT_HELP,
            .help = GEN_COMMAND_HELP,
            .parseArgs = (void *(*) (const int, const char **) ) parseGenerateArgs,
            .execute = (const int (*)(const void *)) executeGenerate,
    };

    return runCommand;
}
