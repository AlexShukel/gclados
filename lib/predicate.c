// Author: Artiom Tretjakovas
// Description: This file contains implementation of functions required for using predicates. Function prototypes are
// described in "predicate.h" file.

#include "predicate.h"

#include <stdlib.h>

void gcladosFreePredicate(GcladosPredicate *predicate) {
    if(predicate->free != NULL) {
        predicate->free(predicate);
    }

    free(predicate->options);
}
