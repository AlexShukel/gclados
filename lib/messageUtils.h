#ifndef __PTF_MESSAGE_UTILS_H__
#define __PTF_MESSAGE_UTILS_H__

#include "stdbool.h"
#include "colors.h"

char* ptfStandardErrorMessage(bool pass, char* usage, char* expected, char* received);

#endif
