#include "../inc/header.h"

static void clearenv(void) {
    extern char **environ;

    for (int i = 0; environ[i]; i++) {
        char **arr = mx_strsplit(environ[i], '=');

        unsetenv(arr[0]);
        mx_del_strarr(&arr);
    }
    unsetenv("XPC_FLAGS");
    unsetenv("SHELL");
    unsetenv("LOGNAME");
    // unsetenv("PATH");
    unsetenv("PWD");
    unsetenv("_");
    unsetenv("TERM");
    unsetenv("SECURITYSESSIONID");
    unsetenv("TERM_PROGRAM");
    unsetenv("LC_CTYPE");
}

static void mx_env_illegal_option(char ch, int fl) {
    if (fl == 0)
        mx_printerr("env: illegal option -- ");
    else
    mx_printerr("option requires an argument -- ");
    write(2, &ch, 1);
    mx_printerr("\n"
                "usage: env [-i] [-P utilpath] [-u name]\n"
                "\t   [name=value] "
                "[utility [argument ...]]\n");
    exit(125);
}

static void first_part(char *s, int *flag, char **next) {
    int i = 2;

    if (s[1] != 'i' && s[1] != 'u' && s[1] != 'P')
        mx_env_illegal_option(s[1], 0);
    else if (s[1] == 'u' || s[1] == 'P') {
        if (s[2] == '\0')
            return;
        else
            for (; s[i]; i++)
                *next = mx_join_char(*next, s[i]);
    }
    else if (s[1] == 'i') {
        *flag = 1;
        clearenv();
        for (; s[i]; i++)
            if (s[i] != 'i')
                mx_env_illegal_option(s[i], 0);
    }
}

static void second_part(char *next, char ***arr, char *s) {
    if (next != NULL) {
        if (s[1] == 'u')
            unsetenv(next);
        else if (s[1] == 'P')
            setenv("PATH", next, 1);
        return;
    }
    char **shift = *arr;

    shift += 1;
    if (!*shift) {
        mx_env_illegal_option(s[1], 1);
        return;
    }
    if (s[1] == 'u')
        unsetenv(*shift);
    else if (s[1] == 'P')
        setenv("PATH", *shift, 1);
    *arr = shift;
}

char **mx_option_loop(char **arr, int *flag) {
    for (; *arr && **arr == '-'; arr += 1) {
        if (mx_strcmp(*arr, "--") == 0)
            return arr += 1;
        char *s = *arr;
        char *next = NULL;

        first_part(s, flag, &next);
        if (s[1] == 'u' || s[1] == 'P')
            second_part(next, &arr, s);
        mx_strdel(&next);
    }
    return arr;
}
