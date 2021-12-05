// Author: Artiom Tretjakovas.
// Description: This file contains the abstract structure for commands to be run through the command line.

#ifndef GCLADOS_COMMAND_H
#define GCLADOS_COMMAND_H

// Structure, containing necessary information for running it via command line.
typedef struct {
    // Command name. Will be used to identify it. Should be unique.
    const char *slug;
    // Text, which will be displayed on "gclados help <command-name>", where <command-name> is specified command slug.
    const char *helperText;
    // Function to parse arguments.
    // Parameters:
    //     argc - argument count, from the **main**.
    //     argv - argument array, from the **main**.
    // Return:
    //     Should return pointer to the options.
    void *(*parseArgs)(const int argc, const char *argv[]);
    // Function, which will be called when user executes program with parameters "gclados <command-name>".
    // Parameters:
    //     *args - pointer to parsed argument array. Arguments from **parseArgs** function.
    // Return:
    //     Should return command status code.
    const int (*execute)(const void *args);
} Command;

#endif