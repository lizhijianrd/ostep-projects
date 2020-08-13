#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TRUE  1
#define FALSE 0

#define BUFF_SIZE 4096

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("wzip: file1 [file2 ...]\n");
        exit(1);
    }
    int n = -1, nread;
    char c;
    char buff[BUFF_SIZE];
    for (int i = 1; i < argc; i++) {
        const char *filename = argv[i];
        FILE *stream = fopen(filename, "r");
        if (stream == NULL) {
            exit(1);
        }
        while (TRUE) {
            nread = fread(buff, sizeof(char), BUFF_SIZE, stream);
            if (nread > 0 && n == -1) {
                c = buff[0];
                n = 0;
            }
            for (int i = 0; i < nread; i++) {
                if (buff[i] == c) {
                    n++;
                } else {
                    fwrite(&n, sizeof(int), 1, stdout);
                    fwrite(&c, sizeof(char), 1, stdout);
                    c = buff[i];
                    n = 1;
                }
            }
            if (nread < BUFF_SIZE) {
                break;
            }
        }
        fclose(stream);
    }
    fwrite(&n, sizeof(int), 1, stdout);
    fwrite(&c, sizeof(char), 1, stdout);
    return 0;
}
