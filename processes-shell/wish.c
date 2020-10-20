#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "builtin.h"
#include "error.h"
#include "path.h"
#include "utils.h"

#define MAX_ARG_CNT 63

pid_t run_command(char *cmd) {
    if (cmd[0] == '\0') { return 0; }

    // parse command
    char *s = NULL;
    char *command = NULL, *redirect = NULL;
    for (int i = 0; TRUE; i++) {
        s = strsep(&cmd, ">");
        if (s == NULL) { break; }
        if (i == 0) {
            command = s;
        } else if (i == 1) {
            redirect = s;
        } else {
            report_error();
#ifdef DEBUG
            fprintf(stderr, "more than one redirect punctuation\n");
#endif
            return 0;
        }
    }

    // parse redirection
    if (redirect != NULL) {
        redirect = trim(redirect);
        if (*redirect == '\0') {
            report_error();
#ifdef DEBUG
            fprintf(stderr, "no redirect file\n");
#endif
            return 0;
        }
        for (char *p = redirect; *p != '\0'; p++) {
            if (*p == ' ') {
                report_error();
#ifdef DEBUG
                fprintf(stderr, "more than one redirect file\n");
#endif
                return 0;
            }
        }
    }

    // parse args
    char *myargs[MAX_ARG_CNT + 1] = {NULL};
    for (int i = 0; i < MAX_ARG_CNT; i++) {
        while ((s = strsep(&command, " ")) != NULL) {
            if (*s != '\0') { break; }
        }
        if (s == NULL) {
            myargs[i] = NULL;
            break;
        }
        myargs[i] = strdup(s);
    }
    myargs[MAX_ARG_CNT] = NULL;
    if (myargs[0] == NULL) {
        report_error();
        return 0;
    }
    
    // builtin commands
    if (strcmp(myargs[0], BUILTIN_CMD_EXIT) == 0) {
        if (myargs[1] != NULL) {
            report_error();
#ifdef DEBUG
            fprintf(stderr, "exit do not need params\n");
#endif
        } else {
            builtin_exit();
        }
        return 0;
    }
    if (strcmp(myargs[0], BUILTIN_CMD_CD) == 0) {
        builtin_cd(myargs[1]);
        return 0;
    } 
    if (strcmp(myargs[0], BUILTIN_CMD_PATH) == 0) {
        builtin_path(myargs + 1);
        return 0;
    }
    if (strcmp(myargs[0], BUILTIN_CMD_PRINT_PATH) == 0) {
        builtin_print_path();
        return 0;
    }
    
    // run command
    pid_t rc = fork();
    if (rc < 0) {
        report_error();
#ifdef DEBUG
        fprintf(stderr, "fork failed\n");
#endif
        return 0;
    } else if (rc == 0) {
        if (redirect != NULL) {
            close(STDOUT_FILENO);
            close(STDERR_FILENO);
            open(redirect, O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR|S_IWUSR);
            dup2(STDOUT_FILENO, STDERR_FILENO);
        }
        char *full_cmd = search_path(myargs[0]);
        if (full_cmd == NULL) {
            report_error();
#ifdef DEBUG
            fprintf(stderr,
                    "no executable file found in path. cmd=%s\n", myargs[0]);
#endif
            exit(0);
        }
        execv(full_cmd, myargs);
    }
    return rc;
}

void wish_line(char *line) {
#ifdef DEBUG
    fprintf(stderr, "[debug] wish_line=%s\n", line);
#endif
    if (line[0] == '\0') { return; }
    char *s = NULL;
    int n = 0;
    while ((s = strsep(&line, "&")) != NULL) {
        s = trim(s);
        pid_t pid = run_command(s);
        if (pid > 0) { n = n + 1; }
    }
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }
}

int wish_batch(const char *filename) {
    FILE *stream = fopen(filename, "r");
    if (stream == NULL) {
        report_error();
#ifdef DEBUG
        fprintf(stderr, "open batch file failed\n");
#endif
        return 1;
    }
    char *buff = NULL;
    size_t len = 0;
    while (getline(&buff, &len, stream) != -1) {
#ifdef DEBUG
        fprintf(stderr, "[debug pid=%d] wish_batch buff=%s\n", getpid(), trim(buff));
#endif
        wish_line(trim(buff));
    }
    free(buff);
    return 0;
}

int wish_interactive() {
    char *buff = NULL;
    size_t len = 0;
    while (TRUE) {
        printf("wish> ");
        if (getline(&buff, &len, stdin) != -1) {
            wish_line(buff);
        } else {
            break;
        }
    }
    return 0;
}

int main(int argc, char **argv) {
    init_path();
    if (argc == 1) {
        return wish_interactive();
    } else if (argc == 2) {
        return wish_batch(argv[1]);
    } else {
        report_error();
#ifdef DEBUG
        fprintf(stderr, "more then one batch file\n");
#endif
        return 1;
    }
    return 0;
}

