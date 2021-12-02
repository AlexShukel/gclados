#include "predicate.h"

#include <stdlib.h>

void gcladosFreePredicate(GcladosPredicate *predicate) {
    if(predicate->free != NULL) {
        predicate->free(predicate);
    }

    free(predicate->options);
}
