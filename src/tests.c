#include <assert.h>
#include <stdlib.h>

#include "logging.h"
#include "utils.h"

static void safe_malloc_1(void) {
    void *tmp = safe_malloc(10);
    assert(tmp);
    free(tmp);
}

void run_tests(void) {
    logs("Hello, world!");
    logs(INFO "Green!");
    logs(WARN "Yellow!");
    logs(FATAL "Red!");

    safe_malloc_1();
    logs(INFO "Passed all tests");
}
