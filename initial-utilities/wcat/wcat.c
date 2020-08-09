#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TRUE  1
#define FALSE 0

#define BUFF_SIZE 4097
#define READ_SIZE BUFF_SIZE - 1

int main(int argc, char **argv) {
    char buff[BUFF_SIZE] = {0};
    for (int i = 1; i < argc; i++) {
        const char *filename = argv[i];
        FILE *file = fopen(filename, "r");
        if (file == NULL) {
            printf("wcat: cannot open file\n");
            exit(1);
        }
        int n = 0;
        while (TRUE) {
            n = fread(buff, sizeof(char), READ_SIZE, file);
            buff[n] = '\0';
            printf("%s", buff);
            if (n < READ_SIZE) {
                if (ferror(file) != 0) {
                    printf("error occur while reading file %s: %s",
                            filename, strerror(ferror(file)));
                }
                break;
            }
        }
        if (fclose(file) == EOF) {
            printf("close file %s failed: %s", filename, strerror(errno));
        }
    }
    return 0;
}

