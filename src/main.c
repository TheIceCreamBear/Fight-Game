#include <stdio.h>

#include "logging.h"
#include "colors.h"

void init(void) {
  log_init();
}

int main(int argc, char **argv) {
  init();
  logs("Hello, world!");
  logs(INFO "Green!");
  logs(WARN "Yellow!");
  logs(FATAL "Red!");
  return 0;
}
