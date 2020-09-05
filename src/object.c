#include <stdlib.h>

#include "object.h"

void object_default_init(void *obj) {
    // do nothing (for now)
}

void object_default_destroy(void *obj) {
    free(obj);
}
