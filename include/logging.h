#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>

FILE *log_output_stream; /* Output stream to log to */
void logs(const char *str); /* Log a message */
void log_init(void); /* Initialize logging resources */

#endif
