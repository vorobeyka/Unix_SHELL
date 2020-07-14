#include "../inc/header.h"

static void error_print_env(char *str) {
    mx_printerr("env: ");
    mx_printerr(str);
    mx_printerr(": No such file or directory\n");
}

static void mx_other_exe(char **argv, t_ost *tost, char *str) {
    int result = 0;

    if (mx_strcmp(argv[0], "./ush") == 0) {
        if (execve(tost->path, argv, tost->env) < 0) {
            perror("ush: execve");
            exit(EXIT_FAILURE);
        }
    }
    else if ((mx_get_char_index(argv[0], '/') != -1
            && execv(argv[0], argv) < 0)
            || (getenv("PATH") && execvp(argv[0], argv) < 0))
        result = 127;
    if ((!getenv("PATH") || !str) && mx_strlen(argv[0]) > 0) {
        mx_long_error_print("ush: command not found: ",
                            argv[0], "\n", NULL);
        exit(EXIT_FAILURE);
    }
    else if (mx_strlen(argv[0]) > 0)
        error_print_env(str);
    exit(result);
}

void mx_exe(char **command, t_ost *tost, char *str) {
    int result = 0;

    if (command && mx_chech_support(command)) {
        char *supp_com = mx_command_find(str);
        
        result = mx_support(tost, command[0], supp_com, str);
        free(supp_com);
        exit(result);
    }
    else if (command && mx_check_command(tost->support, command))
        result = mx_command(tost->support, command[0], tost);
    else if (command && mx_check_builtin(command[0]))
        result = mx_builtin(command, tost);
    else if (command)
        mx_other_exe(command, tost, str);
    exit(result);
}

static void chose_yur_way(int fd[2], char **command, t_ost *tost) {
    wait(NULL);
    close(fd[1]);
    dup2(fd[0], STDIN_FILENO);
    close(fd[0]);
    mx_exe(command, tost, NULL);
}

void mx_pipe_rec(char **arr, int lenn, t_ost *tost) {
    char **command = mx_parse_all(arr[lenn], tost);
    int fd[2];
    pid_t pid;

    if (lenn == 0)
        mx_exe(command, tost, NULL);
    if (pipe(fd) == -1)
        perror("ush: pipe");
    pid = fork();
    if (pid < 0)
        perror("ush: fork");
    else if (pid == 0) {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        mx_pipe_rec(arr, lenn - 1, tost);
    }
    else
        chose_yur_way(fd, command, tost);
}

