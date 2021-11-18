#include <stdio.h>
#include <stdbool.h>
#include "help.h"
#include "run.h"
#include <string.h>
#include "gclados.h"

/*bool isKnownCommand(char* command) {
    return strcmp(helpCmd.slug, command) == 0;
}*/

void parseGeneralArgs(int argc, char *argv[]) {

}

int main(int argc, char *argv[]) {
    Command helpCmd = createHelpCommand();
    Command runCmd = createRunCommand();

    void* opts = helpCmd.parseArgs(argc - 2, argv + 2);

    if(opts != NULL) {
        return helpCmd.execute(opts);
    } else {
        return 0;
    }
}
