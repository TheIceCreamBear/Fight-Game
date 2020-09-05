#include <stdlib.h>

#include "object.h"
#include "room.h"

void __room_init(void *args) {
    // TODO port Go code from joseph's Fight.go
}

void __room_destroy(void *args) {
    object_default_destroy(args);
}

void __room_interact(void *args) {
    Room *self = (Room *) args;
    /* Depending on the type of room and
     * its contents, tell the user what lies
     * within the room */
}

