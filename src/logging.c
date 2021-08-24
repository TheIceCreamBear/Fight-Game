#include <stdio.h>
#include <time.h>

#include "logging.h"
#include "colors.h"


/* Output stream to log to */
FILE *log_output_stream;

/* Log a message */
void logs(const char *str) {
    if (!log_output_stream) {
        fprintf(stderr, "log_init() must be called before logs!\n");
        return;
    }
    fprintf(log_output_stream, "%s" ANSI_RESET "\n", str);
}

/* Manually set the output stream of log messages */
void log_set_ouput_stream(FILE *fp) {
    log_output_stream = fp;
}

/* Default-initialize the outputstream to stdout */
void log_init(void) {
    log_output_stream = stdout; /* defualt output stream */
}
