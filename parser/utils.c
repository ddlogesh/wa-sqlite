#include <stdio.h>
#include "utils.h"

void throwError(const char *errMsg) {
    fprintf(stderr, "ERR: %s\n", errMsg);
}
