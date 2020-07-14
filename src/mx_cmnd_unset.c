#include "../inc/header.h"

static int unset(char **cmnd, int i) {
    if (!i) {
        return 1;
    }
    unsetenv(cmnd[i]);
    return unset(cmnd, i - 1);
}

static int mx_cmnd_unset(t_built *u) {
    int len = mx_len_of_array(u->commands) - 1;

    return unset(u->commands, len);
}

int mx_unset(char **argv, t_ost *tost) {
    tost->built->commands = argv;
    return mx_cmnd_unset(tost->built);;
}
