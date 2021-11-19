#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "gclados.h"
#include "args.h"
#include "help.h"
#include "run.h"

typedef struct {
    bool colors;
} GeneralArguments;

GeneralArguments parseGeneralArguments(int *argc, char* argv[]) {
    Argument generalArguments[] = {
            createBoolArgument("colors", "Enables / disables stdout coloring with ANSI escape codes."),
    };

    void **values = parseArguments(generalArguments, sizeof(generalArguments) / sizeof(Argument), argc, argv);

    GeneralArguments generalArgumentValues;

    if(values[0] != NULL) {
        generalArgumentValues.colors = *((bool*) values[0]);
    } else {
        generalArgumentValues.colors = true;
    }

    return generalArgumentValues;
}

int main(int argc, char *argv[]) {
    if(argc == 1) {
        printGeneralHelp();

        return EXIT_FAILURE;
    }

    char* commandSlug = argv[1];

    argc -= 2;
    argv += 2;

    GeneralArguments arguments = parseGeneralArguments(&argc, argv);
    gcladosColors.setColorsSupported(arguments.colors);

    Command runCmd = createRunCommand();
    Command helpCmd = createHelpCommand();
    Command allCommands[] = {
            helpCmd,
            runCmd,
    };
    Command *currentCommand = NULL;

    int commandCount = sizeof(allCommands) / sizeof(Command);

    for(int i = 0; i < commandCount; ++i) {
        if(strcmp(allCommands[i].slug, commandSlug) == 0) {
            currentCommand = &allCommands[i];
            break;
        }
    }

    if(currentCommand == NULL) {
        char buffer[100];
        sprintf(buffer, "Unknown command received \"%s\". "
                        "Run \"gclados help\" to see list of all available commands", commandSlug);
        gcladosPanic(buffer, EXIT_FAILURE);
    }

    void* opts = currentCommand->parseArgs(argc, argv);

    if(opts != NULL) {
        return currentCommand->execute(opts);
    } else {
        return EXIT_FAILURE;
    }
}
