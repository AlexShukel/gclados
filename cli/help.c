#include "help.h"
#include "stdio.h"
#include "stdlib.h"

// General C language analysis-driven orchestration system.
const char generalHelpText[] = "Usage: gclados <command> [options]\n"
                               "General options:\n"
                               "  --no-colors      Disable colored output. Recommended for terminals, "
                               "which are not supporting ANSI escape codes.\n"
                               "Commands:\n"
                               "  help             Show general help. If there is a command name after, shows help "
                               "for the specified command.\n"
                               "  run              Execute tests.\n";

typedef enum
{
    ALL,
    COMMAND
} HelpFor;

typedef struct {
    HelpFor helpFor;
    struct Command *command;
} HelpCommandOptions;

HelpCommandOptions *parseHelpArgs(int argc, char *argv[]) {
    HelpCommandOptions *options = malloc(sizeof(HelpCommandOptions));

    /* if(argc == 0) */ {
        options->helpFor = ALL;
        options->command = NULL;
    }

    // TODO: implement help for single command

    return options;
}

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
                           (void *(*) (int, char *[])) parseHelpArgs,
                           (int(*)(void *)) executeHelp};

    return helpCommand;
}
