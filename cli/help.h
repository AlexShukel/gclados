// Author: Artiom Tretjakovas
// Description: This file contains the "gclados help" command.

#ifndef GCLADOS_HELP_H
#define GCLADOS_HELP_H

#include "command.h"

// Function, which creates help command instance.
// Returns:
//     Instance of Command structure. Should be used in **main** method.
Command createHelpCommand(Command *commands[], unsigned long commandCount);

// Function, which prints general cli help into stdout.
void printGeneralHelp();

#endif