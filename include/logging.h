#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>

extern FILE *log_output_stream;

void logs(const char *str); /* Log a message */
void log_init(void); /* Initialize logging resources */

#endif
