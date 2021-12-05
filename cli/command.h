// Author: Artiom Tretjakovas.
// Description: This file contains the abstract structure for commands to be run through the command line.

#ifndef GCLADOS_COMMAND_H
#define GCLADOS_COMMAND_H

// Structure, containing necessary information for running it via command line.
typedef struct {
    const char *slug;
    const char *helperText;
    void *(*parseArgs)(const int argc, const char *argv[]);
    const int (*execute)(const void *args);
} Command;

#endif