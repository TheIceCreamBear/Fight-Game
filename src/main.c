#include <stdlib.h>
#include <stdio.h>

#include "logging.h"

#include "example-gl.h"

#include "utils.h"

extern FILE *log_output_stream; /* Output stream to log to */

void init(void) {
    log_init();
}

int main(int argc, char **argv) {
    init();
    simpleGLFWExample();
    run_tests();
    _Exit(EXIT_SUCCESS);
}
