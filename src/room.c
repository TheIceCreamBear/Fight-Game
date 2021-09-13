#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "room.h"

void room_init(void *args) {
    Room *self = (Room *) args;
    self->up = self->down = self->left = self->right = NULL;
    self->rtype = START;
    self->difficulty = rand_in_range(1, 10);

    // TODO import Go code from joseph's Fight.go
}

void room_interact(void *args) {
    Room *self = (Room *) args;

    printf("self: %p\tup: %p\tdown: %p\tleft: %p\tright: %p\n", self, self->up, self->down, self->left, self->right);
    printf("Room type: %s\n", rtype_str(self->rtype));
    printf("Room difficulty: %d\n", self->difficulty);
    /* Depending on the type of room and
     * its contents, tell the user what lies
     * within the room */
}

