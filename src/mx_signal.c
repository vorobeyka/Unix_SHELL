#include "../inc/header.h"

void mx_set_signals(void) {
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
}

int mx_if_else(char **command, int i, t_ost *tost) {
    int result = 0;

    if (mx_pre_check(command[i], tost, 0))
        result = mx_pre_support(tost, command[i]);
    else if (mx_pre_check(command[i], tost, 1))
        result = mx_command(tost->support, command[i], tost);
    else if (mx_first_built_check(command[i]))
        result = pre_builtin(command[i], tost);
    else if (getenv("PATH"))
        result = mx_ush_execute(command[i], tost);
    else {
        mx_long_error_print("ush: command not found: ",
                            command[i], "\n", NULL);
        result = 127;
    }
    return result;
}

void mx_run_shell(char **argv, t_ost *tost, char *str, int fl) {
    if (str && mx_get_substr_index(argv[0], "/ush") > 0) {
        if (fl == 1)
            execve(tost->path, argv, NULL);
        else 
            execve(tost->path, argv, tost->env);
        perror("ush: execve");
        exit(EXIT_FAILURE);
    }
    if (execve(tost->path, argv, tost->env) < 0) {
        perror("ush: execve");
        exit(EXIT_FAILURE);
    }
}

void mx_env_set_grp(char **argv, t_ost *tost, pid_t pid) {
    signal(SIGINT, SIG_DFL);
    signal(SIGTSTP, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
    setpgid(pid, pid);
    if (isatty(0)) {
        tcsetpgrp(0, getpid());
        tcsetpgrp(1, getpid());
    }
    mx_env_part(argv, tost);
}
