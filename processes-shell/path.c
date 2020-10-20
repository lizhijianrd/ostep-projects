#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "path.h"
#include "error.h"


path_t path;

void init_path() {
    path.len = 1;
    path.cap = 8;
    path.max_dir_len = 4;
    path.dir_len = (int *)malloc(sizeof(int) * path.cap);
    path.dirs = (char **)malloc(sizeof(char *) * path.cap);
    path.dirs[0] = strdup("/bin");
    path.dir_len[0] = 4;
}

void builtin_path(char **argv) {
    if (argv == NULL) {
        report_error();
#ifdef DEBUG
        fprintf(stderr, "[error] builtin_path error. argv=NULL\n");
#endif
        return;
    }
    for (int i = 0; i < path.len; i++) {
        free(path.dirs[i]);
        path.dirs[i] = NULL;
    }
    path.len = 0;
    char **p = argv;
    while (*p != NULL) {
        path.len = path.len + 1;
        p = p + 1;
    }
    if (path.len > path.cap) {
        path.cap = path.len;
        free(path.dirs);
        free(path.dir_len);
        path.dirs = (char **)malloc(sizeof(char *) * path.cap);
        path.dir_len = (int *)malloc(sizeof(int) * path.cap);
    }
    p = argv;
    path.max_dir_len = 0;
    for (int i = 0; i < path.len; i++) {
        path.dir_len[i] = strlen(argv[i]);
        path.dirs[i] = strdup(argv[i]);
        if (path.dir_len[i] > path.max_dir_len) {
            path.max_dir_len = path.dir_len[i];
        }
    }
}

void builtin_print_path() {
    if (path.len > 0) { printf("%s", path.dirs[0]); }
    for (int i = 1; i < path.len; i++) { printf(":%s", path.dirs[i]); }
    printf("\n");
}

char *search_path(char *command) {
    int command_len = strlen(command);
    char *buff = 
        (char *)malloc(sizeof(char) * (path.max_dir_len + command_len + 2));
    for (int i = 0; i < path.len; i++) {
        strcpy(buff, path.dirs[i]);
        strcpy(buff + path.dir_len[i], "/");
        strcpy(buff + path.dir_len[i] + 1, command);
        if (access(buff, X_OK) == 0) {
            return buff;
        }
    }
    free(buff);
    return NULL;
}

