#include "../inc/header.h"

static int next_pwd(t_built *u) {
    if (u->commands[1]) {
        mx_printerr("pwd: too many arguments\n");
        return 1;
    } else {
        mx_printstr(u->curctlg);
        mx_printstr("\n");
        return 0;
    }
}

static int mx_cmnd_pwd(t_built *u) {
    char *flags = NULL;

    if (u->commands[1] && u->commands[1][0] == '-') {
        flags = u->commands[1];
        flags++;
        if (*flags == '-' && mx_strlen(flags) > 1) {
            mx_printerr("pwd: --: ilega pparametr\n");
            return 1;
        }
        if (mx_reverse_index(flags, 'L') < mx_reverse_index(flags, 'P'))
            mx_printstr(u->ppwd);
        else
            mx_printstr(u->curctlg);
    }
    else {
        return next_pwd(u);
    }
    mx_printstr("\n");
    return 0;
}

int mx_pwd(char **argv, t_ost *tost) {
    tost->built->commands = argv;
    return mx_cmnd_pwd(tost->built);;
}
