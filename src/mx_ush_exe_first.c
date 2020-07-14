#include "../inc/header.h"

static void child_part(char **argv, int lenn, t_ost *tost) {
    if (isatty(0)) {
        tcsetpgrp(0, getpid());
        tcsetpgrp(1, getpid());
    }
    mx_pipe_rec(argv, lenn - 1, tost);
}

static void parent_part(int *status, pid_t pid, char *command, t_ost **tost) {
    if (isatty(0)) {
        tcsetpgrp(0, getpid());
        tcsetpgrp(1, getpid());
    }
    if (WIFSTOPPED(*status)) {
        *status = 146;
        (*tost)->out = 1;
        mx_long_print("\nush: suspended  ", command, "\n", NULL);
        mx_add_job(command, pid, &(*tost)->jobs);
    }
    else if (WIFSIGNALED(*status)) {
        *status = 130;
        mx_printstr("\n");
    }
    else if (WIFEXITED(*status))
        *status = WEXITSTATUS(*status);
}

static int for_auditor(char **argv) {
    int count = 0;

    for (int i = 0; argv[i]; i++)
        count++;
    return count;
}

int mx_ush_execute(char *command, t_ost *tost) {
    char **argv = mx_parse_pipes_and_tz(command, '|');
    int lenn = for_auditor(argv);
    int status = 0;

    pid_t pid = fork();

    if (pid < 0)
        perror("ush: fork");
    else if (pid == 0) {
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        setpgid(pid, pid);
        child_part(argv, lenn, tost);
    }
    else
        waitpid(pid, &status, WUNTRACED);
    parent_part(&status, pid, command, &tost);
    mx_del_strarr(&argv);
    return status;
}

