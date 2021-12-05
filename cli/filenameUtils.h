// Author: Artiom Tretjakovas
// Description: This file contains useful functions for manipulating file names.

#ifndef GCLADOS_FILENAME_UTILS_H
#define GCLADOS_FILENAME_UTILS_H

// Function, which generates temporary file name with specified extension.
// Parameters:
//     *extension - file ending. Normally, prefixed with dot ("."). Example: ".txt", ".c", etc.
// Returns:
//     Pointer to temporary file name string. Should be disposed after using it, by calling free.
//     Returns NULL if generation failed.
char *tmpNameExtended(char *extension);

#endif
