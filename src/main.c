#include <stdlib.h>
#include <stdio.h>

#include "types.h"
#include "logging.h"

#include "example-gl.h"
#include "example-al.h"
#include "audio.h"

#include "tests.h"
#include "utils.h"

extern FILE *log_output_stream; /* Output stream to log to */

void init(void) {
    log_init();
    audioInit();
}

int main(int argc, char **argv) {
    init();
    simpleALExample();
    simpleGLFWExample();
    cleanUpAllAL();
    run_tests();
    _Exit(EXIT_SUCCESS);
}
