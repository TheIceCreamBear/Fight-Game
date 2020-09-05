#include "utils.h"

int rand_in_range(int n, int m) {
    return m + rand() / (RAND_MAX / (n - m + 1) + 1);
}
