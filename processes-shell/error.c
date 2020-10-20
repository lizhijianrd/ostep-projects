#include <unistd.h>

#include "error.h"

void report_error() {
    const static char error_message[] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, sizeof(error_message) - 1);
}

