#ifndef GCLADOS_ARGS_H
#define GCLADOS_ARGS_H

typedef struct {
    int elementsParsed;
    void *parsedValue;
} ArgumentParseResult;

typedef struct Argument {
    char *name;
    char *description;
    ArgumentParseResult *(*tryParse)(struct Argument *argument, int argc, char *argv[]);
} Argument;

void **parseArguments(Argument *arguments, int argumentCount, int *argc, char *argv[]);

Argument createBoolArgument(char *name, char *description);

#endif
