#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtin.h"
#include "error.h"
#include "utils.h"


void builtin_cd(char *path) {
    if (path == NULL) {
        report_error();
    } else if (chdir(path) != 0) {
        report_error();
    }
}

void builtin_exit() {
    exit(0);
}

