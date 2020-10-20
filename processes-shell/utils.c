#include <string.h>
#include <unistd.h>

#include "utils.h"

char *trim(char *s) {
    int n = strlen(s);
    char *p = s + n - 1;
    while (p >= s && (*p == ' ' || *p == '\n')) {
        *p = '\0';
        p--;
    }
    while (s <= p && (*s == ' ' || *s == '\n')) {
        *s = '\0';
        s++;
    }
    return s;
}

