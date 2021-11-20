#include "args.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void **parseArguments(Argument *arguments, int argumentCount, int *argc, char *argv[]) {
    int currentIndex = 0;
    void **result = calloc(argumentCount, sizeof(void *));

    char *notParsedArgs[*argc];
    int notParsedArgCount = 0;

    while(currentIndex < *argc) {

        if(strcmp(argv[currentIndex], "--") == 0) {
            break;
        }

        bool argumentParsed = false;

        for(int i = 0; i < argumentCount; ++i) {
            ArgumentParseResult *parseResult =
                    arguments[i].tryParse(&arguments[i], *argc - currentIndex, argv + currentIndex);

            if(parseResult != NULL) {
                argumentParsed = true;
                result[i] = parseResult->parsedValue;
                currentIndex += parseResult->elementsParsed;
            }
        }

        if(!argumentParsed) {
            notParsedArgs[notParsedArgCount++] = argv[currentIndex++];
        }
    }

    for(int i = 0; i < notParsedArgCount; ++i) {
        argv[i] = notParsedArgs[i];
    }

    int argumentCountAfterHyphen = *argc - currentIndex;

    if(currentIndex + 1 < *argc) {
        memmove(argv + notParsedArgCount, argv + currentIndex, (*argc - currentIndex) * sizeof(char *));
    }

    *argc = notParsedArgCount + argumentCountAfterHyphen;

    return result;
}

ArgumentParseResult *tryParseBoolArgument(Argument *argument, int argc, char *argv[]) {
    char *fullArgumentName = calloc(strlen(argument->name) + 2, sizeof(char));
    strcat(fullArgumentName, "--");
    strcat(fullArgumentName, argument->name);

    int comparisonResult = strcmp(fullArgumentName, argv[0]);

    free(fullArgumentName);

    if(comparisonResult == 0) {
        bool *value = malloc(sizeof(bool *));
        ArgumentParseResult *result = malloc(sizeof(ArgumentParseResult));

        if(argc > 1 && strcmp("false", argv[1]) == 0) {
            *value = false;
            result->elementsParsed = 2;
        } else if(argc > 1 && strcmp("true", argv[1]) == 0) {
            *value = true;
            result->elementsParsed = 2;
        } else {
            *value = true;
            result->elementsParsed = 1;
        }
        result->parsedValue = value;

        return result;
    }

    return NULL;
}

Argument createBoolArgument(char *name, char *description) {
    Argument newArgument = {
            .name = name,
            .description = description,
            .tryParse = tryParseBoolArgument,
    };

    return newArgument;
}