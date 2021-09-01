#include <stdlib.h>
#include <stdio.h>

#include "types.h"
#include "logging.h"

#include "example-gl.h"

#include "utils.h"

FILE *log_output_stream; /* Output stream to log to */

void init(void) {
    log_init();
}

void allocated_or_die(void *arg) {
    if (!arg) {
        fprintf(stderr, FATAL "malloc() returned null! Aborting\n");
        _Exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv) {
    init();
    simpleGLFWExample();

    Room start;
    Room *iterator = &start;
    
    logs("Hello, world!");
    logs(INFO "Green!");
    logs(WARN "Yellow!");
    logs(FATAL "Red!");

    __room_init(&start);
    for (int i = 0; i < 9; i++) {
        Room *r = malloc(sizeof(Room));
        __room_init(r);
        r->down = iterator;
        iterator->up = r;
        iterator = r;
    }

    for (iterator = &start; iterator; iterator = iterator->up) {
        __room_interact(iterator);
    }
    _Exit(EXIT_SUCCESS);
}
