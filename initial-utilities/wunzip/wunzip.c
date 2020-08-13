#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TRUE  1
#define FALSE 0

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("wunzip: file1 [file2 ...]\n");
        exit(1);
    }
    int n, nread;
    char c;
    for (int i = 1; i < argc; i++) {
        const char *filename = argv[i];
        FILE *stream = fopen(filename, "r");
        if (stream == NULL) {
            exit(1);
        }
        while (TRUE) {
            nread = fread(&n, sizeof(int), 1, stream);
            if (nread == 0) {
                break;
            }
            fread(&c, sizeof(char), 1, stream);
            for (int j = 0; j < n; j++) {
                printf("%c", c);
            }
        }
        fclose(stream);
    }
    return 0;
}
