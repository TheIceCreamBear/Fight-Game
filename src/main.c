#include <stdio.h>

#include "types.h"
#include "logging.h"
#include "colors.h"

void init(void) {
    log_init();
}

int main(int argc, char **argv) {
    Room r;

    init();
    __room_init(&r);
    r.obj.init(&r);

    logs("Hello, world!");
    logs(INFO "Green!");
    logs(WARN "Yellow!");
    logs(FATAL "Red!");
    return 0;
}
