#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>

/* <- - - - - - - - - - TTY color printing - - - - - - - - - -> */
#define TTY_COLOR_ENABLED

#ifdef TTY_COLOR_ENABLED // TODO proper guard, only some tty devices support this functionality
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

struct location {
    int x;
    int y;
};

int rand_in_range(int n, int m);
void *safe_malloc(size_t size); // guaranteed malloc(), or die

// string comparision case insensitive with a max size n
int strincmp(const char* a, const char* b, int n);

// integer based, recurssive pow function, pow must be positive
int powi(int base, int pow);

#endif
