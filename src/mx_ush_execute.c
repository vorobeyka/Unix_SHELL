#include "header.h"

static void mx_other_exe(char **argv, t_ost *tost) {
    if (mx_strcmp(argv[0], "./ush") == 0) {
        if (execve(tost->path, argv, tost->env) < 0) {
            perror("ush: execve");
            exit(EXIT_FAILURE);
        }
    }
    else if (execvp(argv[0], argv) < 0) {
        mx_long_error_print("ush: command not found: ", argv[0], "\n", NULL);
        exit(EXIT_FAILURE);
    }
}

void mx_exe(char **command, t_ost *tost) {
    if (mx_chech_support(command[0])) {
        int result = mx_support(tost, command[0], command[mx_find_comamnd(command)]);

        exit(result);
    }
    else if (mx_check_command(tost->support, command[0])) {
        int result = mx_command(tost->support, command[0], tost);

        exit(result);
    }
    else if (mx_check_builtin(command[0])) {
        int result = mx_builtin(command, tost);

        exit(result);
    }
    else
        mx_other_exe(command, tost);
}

void mx_pipe_rec(char **arr, int lenn, t_ost *tost) {
    char **command = mx_parse_all(arr[lenn], tost); // pars
    int fd[2];

    if (lenn == 0)
        mx_exe(command, tost);
    if (pipe(fd) == -1)
        perror("ush: pipe");
    pid_t pid = fork();

    if (pid < 0)
        perror("ush: fork");
    // Child
    else if (pid == 0) {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        mx_pipe_rec(arr, lenn - 1, tost);
    }
    // Parent
    else {
        wait(NULL);
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        mx_exe(command, tost);
    }
}

int mx_ush_execute(char *command, t_ost *tost) {
    char **argv = mx_parse_pipes_and_tz(command, '|');//mx_trim_split(command, '|'); // pars |
    int lenn = 0;
    int status = 0;

    for (int i = 0; argv[i]; i++)
        lenn++;
    pid_t pid = fork();

    if (pid < 0)
        perror("ush: fork");
    else if (pid == 0) {
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        setpgid(pid, pid);
        tcsetpgrp(0, getpid());
        tcsetpgrp(1, getpid());
        mx_pipe_rec(argv, lenn - 1, tost);
    }
    else
        waitpid(pid, &status, WUNTRACED);
    tcsetpgrp(0, getpid());
    tcsetpgrp(1, getpid());
    if (WIFSTOPPED(status)) {
        status = 146;
        tost->out = 1;
        mx_long_print("\nush: suspended  ", command, "\n", NULL);
        mx_add_job(command, pid, &tost->jobs);
    }
    else if (WIFSIGNALED(status)) {
        status = 130;
        mx_printstr("\n");
    }
    else if (WIFEXITED(status))
        status = WEXITSTATUS(status);
    mx_del_strarr(&argv);
    return status;
}
