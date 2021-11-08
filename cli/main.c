#include <stdio.h>
#include <stdbool.h>
#include "help.h"
#include <string.h>

/*bool isKnownCommand(char* command) {
    return strcmp(helpCmd.slug, command) == 0;
}*/

int main(int argc, char *argv[]) {

    struct Command helpCmd = createHelpCommand();

    void* opts = helpCmd.parseArgs(argc, argv);
    helpCmd.execute(opts);

    return 0;
}
