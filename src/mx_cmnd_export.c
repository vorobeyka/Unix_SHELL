#include "../inc/header.h"

static char **sort_export_env() {
    extern char **environ;
    char **rez = mx_array_copy_strs(environ);
    int flag = 1;
    
    while (flag) {
        flag = 0;
        for (int i = 0; rez[i + 1]; i++) {
            if (mx_strcmp(rez[i], rez[i + 1]) > 0) {
                char *tmp = rez[i];

                rez[i] = rez[i + 1];
                rez[i + 1] = tmp;
                flag = 1;
            }
        }
    }
    return rez;
}

static int print_export() {
    char **env = sort_export_env();

    for (int i = 0; env[i]; i++) {
        mx_long_print(env[i], "\n", NULL, NULL);
        free(env[i]);
    }
    free(env);
    return 0;
}

static int mx_cmnd_export(t_built *u, t_history *history) {
    int len = mx_len_of_array(u->commands) - 1;
    int error_value = 0;
    char **commands = u->commands;
    t_history *lol = NULL;

    lol = history;
    if (!len)
        return print_export();
    return mx_cmnd_export_recurs(commands, len, &error_value);
}

int mx_export(char **argv, t_ost *tost) {
    tost->built->commands = argv;
    return mx_cmnd_export(tost->built, tost->history);
}
