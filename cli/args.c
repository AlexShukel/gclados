// Author: Artiom Tretjakovas
// Description: File containing the implementation of the argument parsing functions. Function prototypes described in
//              header file (args.h)

#include "args.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void **parseArguments(const Argument arguments[], const int argumentCount, int *argc, char *argv[]) {
    int currentIndex = 0;
    void **result = calloc(argumentCount, sizeof(void *));

    // Saving not parsed arguments.
    char *notParsedArgs[*argc];
    // Counting not parsed arguments.
    int notParsedArgCount = 0;

    while(
            // Not all arguments parsed yet.
            currentIndex < *argc &&
            // Encountered double hyphen.
            strcmp(argv[currentIndex], "--") != 0) {

        bool argumentParsed = false;

        // Iterating through all specified arguments.
        for(int i = 0; i < argumentCount; ++i) {
            // Trying to parse argument.
            ArgumentParseResult *parseResult =
                    arguments[i].tryParse(&arguments[i], *argc - currentIndex, (const char **) argv + currentIndex);

            // Parsing result is not NULL - argument successfully parsed.
            if(parseResult != NULL) {
                argumentParsed = true;
                result[i] = parseResult->parsedValue;
                currentIndex += parseResult->elementsParsed;
            }
        }

        // If argument was not parsed, saving it.
        if(!argumentParsed) {
            notParsedArgs[notParsedArgCount++] = argv[currentIndex++];
        }
    }

    // Replacing old arguments with not parsed ones.
    for(int i = 0; i < notParsedArgCount; ++i) {
        argv[i] = notParsedArgs[i];
    }

    // Counting arguments after double hyphen.
    int argumentCountAfterHyphen = *argc - currentIndex;

    if(currentIndex + 1 < *argc) {
        // Moving all arguments after double hyphen.
        memmove(argv + notParsedArgCount, argv + currentIndex, (*argc - currentIndex) * sizeof(char *));
    }

    // Saving new argument count.
    *argc = notParsedArgCount + argumentCountAfterHyphen;

    return result;
}

// Function for parsing boolean argument.
ArgumentParseResult *tryParseBoolArgument(const Argument *argument, const int argc, const char *argv[]) {
    // Prefixing argument name with double hyphen.
    char *fullArgumentName = calloc(strlen(argument->name) + 2, sizeof(char));
    strcat(fullArgumentName, "--");
    strcat(fullArgumentName, argument->name);

    // Checking argument name.
    int comparisonResult = strcmp(fullArgumentName, argv[0]);

    free(fullArgumentName);

    if(comparisonResult == 0) {
        bool *value = malloc(sizeof(bool *));
        ArgumentParseResult *result = malloc(sizeof(ArgumentParseResult));

        // Checking "--<argument-name> false"
        if(argc > 1 && strcmp("false", argv[1]) == 0) {
            *value = false;
            result->elementsParsed = 2;
            // Checking "--<argument-name> true"
        } else if(argc > 1 && strcmp("true", argv[1]) == 0) {
            *value = true;
            result->elementsParsed = 2;
            // Checking "--<argument-name>"
        } else {
            *value = true;
            result->elementsParsed = 1;
        }
        result->parsedValue = value;

        return result;
    }

    // Parsing failed, returning NULL
    return NULL;
}

Argument createBoolArgument(const char *name, const char *description) {
    Argument newArgument = {
            .name = name,
            .description = description,
            .tryParse = tryParseBoolArgument,
    };

    return newArgument;
}