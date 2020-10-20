#ifndef __PATH_H__
#define __PATH_H__

#define BUILTIN_CMD_PATH "path"
#define BUILTIN_CMD_PRINT_PATH "print_path"

typedef struct __path_t {
    int len;
    int cap;
    int max_dir_len;
    int *dir_len;
    char **dirs;
} path_t;

extern path_t path;

void init_path();
void builtin_path(char **);
void builtin_print_path();
char *search_path(char *);

#endif // __PATH_H__

