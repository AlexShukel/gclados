// Author: Artiom Tretjakovas
// Description: This file contains functions to simplify the parsing of command line arguments.

#ifndef GCLADOS_ARGS_H
#define GCLADOS_ARGS_H

// Structure which saves parsed argument values.
// Necessary for internal purposes.
typedef struct {
    // This variable contains the count of strings that were parsed.
    int elementsParsed;
    // This variable contains the pointer to parsed argument value.
    void *parsedValue;
} ArgumentParseResult;

// Structure which describes command line argument.
typedef struct Argument {
    // Name of command line argument.
    const char *name;
    // Description of argument (it's recommended to describe the usage / purpose of argument here).
    const char *description;
    // Function, which parses argument. Will return NULL if parsing failed.
    // Parameters:
    //     *argument - pointer to the Argument structure from which the function is called.
    //     argc      - raw argument count, from the **main** method.
    //     *argv[]   - raw argument array of strings, from the **main** method.
    // Returns:
    //     ArgumentParseResult structure.
    ArgumentParseResult *(*tryParse)(const struct Argument *argument, const int argc, const char *argv[]);
} Argument;

// Function, which parses specified command line arguments.
// Parameters:
//     arguments[]   - array of arguments, which will be used to parse raw arguments.
//     argumentCount - the length of arguments array (first parameter).
//     *argc         - pointer to raw argument count, from the **main** method. Will be mutated.
//     *argv[]       - pointer to the array of strings (raw arguments), from the **main** method. Will be
//                            mutated.
// Returns:
//     Pointer to the array of void*. This array contains parsed argument values. This array should be utilized after
//     usage, by calling free function for each parsed argument and whole array.
// Notes:
//     * This function mutates argc and argv arguments. In the result, argv will contain all not parsed arguments, and
//       argc will contain new array size.
//     * Argument parsing is being stopped after encountering double hyphen (--). Required for passing raw arguments
//       into subprocesses. Example: gclados run hello.test.c -- <gcc arguments>.
void **parseArguments(const Argument arguments[], int argumentCount, int *argc, char *argv[]);

// Function, which creates new boolean argument (flag).
// Parameters:
//     *name        - argument name. Described in the UX section where referred to as <argument-name>
//     *description - description of argument. It's recommended to describe the usage / purpose of argument there.
// Returns:
//     Variable of type Argument. To parse this argument, pass it to **parseArguments** function.
// UX:
//     User can toggle this argument value by specifying --<argument-name> <value> (where <argument-name>> is specified
//     name, and <value> is literal strings "true" or "false") in command line. Also, to turn on this flag, short syntax
//     could be used: --<argument-name>.
Argument createBoolArgument(const char *name, const char *description);

#endif
