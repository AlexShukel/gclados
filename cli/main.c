#include <stdio.h>
#include <stdbool.h>
#include "help.h"
#include "run.h"
#include <string.h>

/*bool isKnownCommand(char* command) {
    return strcmp(helpCmd.slug, command) == 0;
}*/

int main(int argc, char *argv[]) {

    struct Command helpCmd = createHelpCommand();
    struct Command runCmd = createRunCommand();

    void* opts = runCmd.parseArgs(argc - 2, argv + 2);

    return runCmd.execute(opts);
}
