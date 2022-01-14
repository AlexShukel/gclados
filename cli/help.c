// Author: Artiom Tretjakovas
// Description: This file contains implementation of "gclados help" command. Function prototypes is in "help.h"
//              file.

#include "help.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const size_t USAGE_BUFFER_SIZE = 128;
const size_t COMMAND_LIST_BUFFER_SIZE = 4096;
const char COMMAND_PLACEHOLDER[] = "<command>";
const char USAGE_TEXT_TEMPLATE[] = "gclados %s [options]";

const char GENERAL_HELP_TEMPLATE[] =
        "Usage: %s\n"
        "General options:\n"
        "  --colors - Enables / disables colored mode. Recommended for terminals, which do not support them.\n"
        "Commands:\n"
        "%s";

const char COMMAND_HELP_TEMPLATE[] = "Usage: %s\n\n"
                                     "Description: \n"
                                     "%s\n";

// Command specific strings
const char HELP_COMMAND_SLUG[] = "help";
const char HELP_COMMAND_SHORT_HELP[] = "Show general / command-specific help messages.";
const char HELP_COMMAND_HELP[] =
        "Shows useful messages about cli usage, as well as a full list of available commands.\n"
        "Specify command name to display help message for command.\n"
        "For example:\n"
        "  gclados help run\n"
        "Will display useful information about \"run\" command.";

// Exceptions
const char ECOMMAND_NOT_FOUND[] = "Command \"%s\" not found. "
                                  "Please run \"gclados help\" to see a full list of available commands.\n";


// Enum, which is used to determine the type of help that should be displayed.
typedef enum
{
    ALL,
    COMMAND
} HelpFor;

// Structure, which saves options for the help command.
typedef struct {
    HelpFor helpFor;
    Command *command;
} HelpCommandOptions;

Command **globalCommands;
size_t globalCommandCount;

// Function used to parse arguments for the help command.
HelpCommandOptions *parseHelpArgs(int argc, char *argv[]) {
    HelpCommandOptions *options = malloc(sizeof(HelpCommandOptions));

    if(argc == 0) {
        options->helpFor = ALL;
        options->command = NULL;

        return options;
    }

    for(size_t i = 0; i < globalCommandCount; ++i) {
        if(strcmp(globalCommands[i]->slug, argv[0]) == 0) {
            options->helpFor = COMMAND;
            options->command = globalCommands[i];

            return options;
        }
    }

    fprintf(stderr, ECOMMAND_NOT_FOUND, argv[0]);

    return NULL;
}

// Function which constructs string of specific command usage.
void getUsageText(const char *commandSlug, char *output) {
    if(commandSlug == NULL) {
        commandSlug = COMMAND_PLACEHOLDER;
    }

    sprintf(output, USAGE_TEXT_TEMPLATE, commandSlug);
}

// Function which constructs list of all available commands, with short descriptions.
void getCommandList(const Command *commands[], size_t commandCount, char *output) {
    size_t bufferOffset = 0;

    for(size_t i = 0; i < commandCount; ++i) {
        bufferOffset += sprintf(output + bufferOffset, "%10s - %s\n", commands[i]->slug, commands[i]->shortHelp);
    }
}

// Function which prints general help text.
void printGeneralHelp() {
    char usageText[USAGE_BUFFER_SIZE];
    char commandList[COMMAND_LIST_BUFFER_SIZE];

    getUsageText(NULL, usageText);
    getCommandList((const Command **) globalCommands, globalCommandCount, commandList);

    printf(GENERAL_HELP_TEMPLATE, usageText, commandList);
}

void printCommandHelp(const Command *command) {
    char usageText[USAGE_BUFFER_SIZE];
    getUsageText(command->slug, usageText);
    printf(COMMAND_HELP_TEMPLATE, usageText, command->help);
}

int executeHelp(HelpCommandOptions *options) {
    if(options->helpFor == ALL) {
        printGeneralHelp();
    } else if(options->helpFor == COMMAND) {
        printCommandHelp(options->command);
    } else {
        return 1;
    }

    return 0;
}

Command createHelpCommand(Command *commands[], size_t commandCount) {
    globalCommandCount = commandCount;
    globalCommands = commands;

    Command helpCommand = {
            .slug = HELP_COMMAND_SLUG,
            .help = HELP_COMMAND_HELP,
            .shortHelp = HELP_COMMAND_SHORT_HELP,
            .parseArgs = (void *(*) (const int, const char **) ) parseHelpArgs,
            .execute = (const int (*)(const void *)) executeHelp,
    };

    return helpCommand;
}
