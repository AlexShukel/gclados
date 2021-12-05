// Author: Artiom Tretjakovas
// Description: This file contains implementation of "gclados help" command. Function prototypes is in "help.h"
//              file.

#include "help.h"

#include <stdio.h>
#include <stdlib.h>

const char generalHelpText[] = "Usage: gclados <command> [options]\n"
                               "General options:\n"
                               "  --no-colors      Disable colored output. Recommended for terminals, "
                               "which are not supporting ANSI escape codes.\n"
                               "Commands:\n"
                               "  help             Show general help. If there is a command name after, shows help "
                               "for the specified command.\n"
                               "  run              Execute tests.\n"
                               "  generate         Only generate entrypoint for tests.\n";

// Enum, which is used to determine the type of help that should be displayed.
typedef enum
{
    ALL,
    COMMAND
} HelpFor;

// Structure, which saves options for the help command.
typedef struct {
    HelpFor helpFor;
    struct Command *command;
} HelpCommandOptions;

// Function used to parse arguments for the help command.
HelpCommandOptions *parseHelpArgs(int argc, char *argv[]) {
    HelpCommandOptions *options = malloc(sizeof(HelpCommandOptions));

    /* if(argc == 0) */ {
        options->helpFor = ALL;
        options->command = NULL;
    }

    // TODO: implement help for single command

    return options;
}

// Function which prints general help text.
void printGeneralHelp() {
    printf("%s", generalHelpText);
}

int executeHelp(HelpCommandOptions *options) {
    if(options->helpFor == ALL) {
        printGeneralHelp();
    }

    // TODO: implement help for single command

    return 0;
}

Command createHelpCommand() {
    Command helpCommand = {"help",
                           "This is help command",
                           (void *(*) (const int, const char **) ) parseHelpArgs,
                           (const int (*)(const void *)) executeHelp};

    return helpCommand;
}
