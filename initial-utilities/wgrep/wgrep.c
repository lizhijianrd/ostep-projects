#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void grep(FILE *stream, const char *target) {
    char *buff = NULL;
    size_t len = 0;
    ssize_t nread = 0;
    while ((nread = getline(&buff, &len, stream)) != -1) {
        if (strstr(buff, target) != NULL) {
            printf("%s", buff);
        }
    }
    free(buff);
}

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("wgrep: searchterm [file ...]\n");
        exit(1);
    }
    const char *target = argv[1];
    if (argc == 2) {
        grep(stdin, target);
    } else {
        for (int i = 2; i < argc; i++) {
            const char *filename = argv[i];
            FILE *stream = fopen(filename, "r");
            if (stream == NULL) {
                printf("wgrep: cannot open file\n");
                exit(1);
            }
            grep(stream, target);
            fclose(stream);
        }
    }
    return 0;
}
