#include "help.h"
#include "stdlib.h"
#include "stdio.h"

// TODO: set normal helper text
const char generalHelpText[] = "This is general help text. LUL!";

enum HelpFor {
    ALL, COMMAND
};

struct HelpCommandOptions {
    enum HelpFor helpFor;
    struct Command *command;
};

struct HelpCommandOptions* parseHelpArgs(int argc, char *argv[]) {

    struct HelpCommandOptions *options = malloc(sizeof(struct HelpCommandOptions));

    /* if(argc == 0) */ {
        options->helpFor = ALL;
        options->command = NULL;
    }

    // TODO: implement help for single command

    return options;
}

int executeHelp(struct HelpCommandOptions* options) {
    if(options->helpFor == ALL) {
        printf("%s", generalHelpText);
    }

    // TODO: implement help for single command

    return 0;
}

struct Command createHelpCommand() {
    struct Command helpCommand = {
            "help",
            "This is help command",
            (void* (*)(int, char*[])) parseHelpArgs,
            (int (*)(void*)) executeHelp
    };

    return helpCommand;
}
