#include <stdio.h>
#include <time.h>

#include "logging.h"
#include "colors.h"

void logs(const char *str) {
    if (!log_output_stream) {
        fprintf(stderr, "log_init() must be called before logs!\n");
        return;
    }
    fprintf(log_output_stream, "%s" ANSI_RESET "\n", str);
}

void log_init(void) {
    log_output_stream = stdout; /* defualt output stream */
}
