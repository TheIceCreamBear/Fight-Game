#include <errno.h>
#include <ctype.h>

#include "utils.h"
#include "logging.h"

#ifndef COLORS_H
#define COLORS_H

/* colors.h 
 *
 * EXAMPLE:
 *      RED("this text will be red")
 */

#define ANSI_RESET                 "\x1b[0m"

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"

#define ANSI_RED(text)            ("\x1b[31m" text ANSI_RESET)
#define ANSI_GREEN(text)          ("\x1b[32m" text ANSI_RESET)
#define ANSI_YELLOW(text)         ("\x1b[33m" text ANSI_RESET)
#define ANSI_BLUE(text)           ("\x1b[34m" text ANSI_RESET)
#define ANSI_MAGENTA(text)        ("\x1b[35m" text ANSI_RESET)
#define ANSI_CYAN(text)           ("\x1b[36m" text ANSI_RESET)

#define INFO GREEN
#define WARN YELLOW
#define FATAL RED

#endif

extern FILE *log_output_stream;

int rand_in_range(int n, int m) {
    return m + rand() / (RAND_MAX / (n - m + 1) + 1);
}

void *safe_malloc(size_t size) {
    if (!log_output_stream) {
        log_init();
    }

    void *ret = malloc(size);
    if (!ret) {
        logs(FATAL "malloc() failed - Is the system out of memory?\n");
        exit(ENOMEM); // TODO portability
    }
    return ret;
}

// string comparision case insensitive with a max size n
int strincmp(const char* a, const char* b, int n) {
    for (int i = 0; i < n; i++) {
        int d = tolower(a[i]) - tolower(b[i]);
        if (d != 0) {
            return d;
        }
    }
    return 0;
}

// integer based, recurssive pow function, pow must be positive
int powi(int base, int pow) {
    // if pow is 0 or lower, return 1
    if (pow < 1) {
        return 1;
    }

    return base * powi(base, pow - 1);
}
