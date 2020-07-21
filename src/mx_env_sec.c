#include "../inc/header.h"

static bool check_env(char *src) {
    char **s = mx_strsplit(src, '=');

    if (mx_lenn_mass(s) != 2) {
        mx_del_strarr(&s);
        return true;
    }
    mx_del_strarr(&s);
    return false;
}

char **mx_add_env(char **shift) {
    if (shift && *shift)
        while (1) {
            if (!*shift || check_env(*shift))
                return shift;
            putenv(*shift);
            shift += 1;
        }
    return shift;
}

static void mx_print_env(void) {
    extern char **environ;

    for (int i = 0; environ[i]; i++) {
        mx_printstr(environ[i]);
        mx_printstr("\n");
    }
}

void mx_env_part(char **argv, t_ost *tost) {
    char **shift = argv;
    int flag = 0;

    shift++;
    shift = mx_option_loop(shift, &flag);
    shift = mx_add_env(shift);
    if (*shift && mx_strcmp(*shift, "env") != 0)
        mx_exe(shift, tost, *shift, flag);
    else if (flag != 1)
        mx_print_env();
    exit(0);
}

int mx_env(char **argv, t_ost *tost) {
    if (mx_lenn_mass(argv) == 1) {
        mx_print_env();
        return 0;
    }
    pid_t pid = fork();
    int status = 0;

    if (pid < 0)
        perror("ush: fork");
    else if (pid == 0)
        mx_env_set_grp(argv, tost, pid);
    else
        wait(&status);
    if (isatty(0)) {
        tcsetpgrp(0, getpid());
        tcsetpgrp(1, getpid());
    }
    if (WIFEXITED(status))
        status = WEXITSTATUS(status);
    return status;
}

