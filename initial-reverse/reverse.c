#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define TRUE  1
#define FALSE 0

#define STACK_INIT_BUFF_SIZE 32

void err_quit(const int code, const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
    exit(code);
}

typedef struct {
    int len;
    int cap;
    char **buff;
} stack;

stack *stack_init() {
    stack *sp = (stack *) malloc(sizeof(stack));
    if (sp == NULL) {
        err_quit(1, "malloc failed\n");
    }
    sp->buff = (char **) malloc(sizeof(char *) * STACK_INIT_BUFF_SIZE);
    if (sp->buff == NULL) {
        err_quit(1, "malloc failed\n");
    }
    sp->cap = STACK_INIT_BUFF_SIZE;
    sp->len = 0;
    return sp;
}

void stack_resize(stack *sp) {
    sp->cap = sp->cap * 2;
    sp->buff = (char **) realloc(sp->buff, sp->cap);
    if (sp->buff == NULL) {
        err_quit(1, "malloc failed\n");
    }
}

void stack_push(stack *sp, char *val) {
    if (sp->len == sp->cap) {
        stack_resize(sp);
    }
    sp->buff[sp->len] = val;
    sp->len = sp->len + 1;
}

char *stack_pop(stack *sp) {
    if (sp->len > 0) {
        sp->len = sp->len - 1;
        return sp->buff[sp->len];
    }
    return NULL;
}

int stack_is_empty(const stack *sp) {
    return sp->len == 0;
}

void reverse(FILE *stream_input, FILE *stream_output) {
    stack *sp = stack_init();
    char *buff = NULL;
    size_t len = 0;
    ssize_t nread = 0;
    while ((nread = getline(&buff, &len, stream_input)) != -1) {
        stack_push(sp, buff);
        buff = NULL;
    }
    while (!stack_is_empty(sp)) {
        buff = stack_pop(sp);
        fputs(buff, stream_output);
        free(buff);
    }
}

int is_same_file(const char *file1, const char *file2) {
    struct stat stat1, stat2;
    if (stat(file1, &stat1) == -1) {
        err_quit(1, "reverse: cannot open file '%s'\n", file1);
    }
    if (stat(file2, &stat2) == -1) {
        return FALSE;
    }
    return stat1.st_ino == stat2.st_ino;
}

int main(int argc, char **argv) {
    FILE *stream_input, *stream_output;
    if (argc == 1) {
        stream_input = stdin;
        stream_output = stdout;
    } else if (argc == 2) {
        const char *filename_input = argv[1];
        stream_input = fopen(filename_input, "r");
        if (stream_input == NULL) {
            err_quit(1, "reverse: cannot open file '%s'\n", filename_input);
        }
        stream_output = stdout;
    } else if (argc == 3) {
        const char *filename_input = argv[1];
        const char *filename_output = argv[2];
        if (is_same_file(filename_input, filename_output)) {
            err_quit(1, "reverse: input and output file must differ\n");
        }
        stream_input = fopen(filename_input, "r");
        if (stream_input == NULL) {
            err_quit(1, "reverse: cannot open file '%s'\n", filename_input);
        }
        stream_output = fopen(filename_output, "w");
        if (stream_input == NULL) {
            err_quit(1, "reverse: cannot open file '%s'\n", filename_output);
        }
    } else {
        err_quit(1, "usage: reverse <input> <output>\n");
    }
    reverse(stream_input, stream_output);
    fclose(stream_input);
    fclose(stream_output);
    return 0;
}
