#include "utils.h"

void throw_error(const char *err_msg) {
    fprintf(stderr, "ERR: %s\n", err_msg);
}
