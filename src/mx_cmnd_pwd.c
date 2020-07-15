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

static bool check_error_flags(char *flags) {
    if (flags[0] == '-')
        flags++;
    for ( ; flags && *flags; flags++) {
        if (*flags != 'P' && *flags != 'L') {
            mx_printerr("pwd: bad option: -");
            write(2, flags, 1);
            mx_printerr("\n");
            return true;
        }
    }
    return false;
}

static int mx_cmnd_pwd(t_built *u) {
    char *flags = NULL;

    if (u->commands[1] && u->commands[1][0] == '-') {
        flags = u->commands[1];
        flags++;
        if (check_error_flags(flags)) {
            return 1;
        }
        if (mx_get_char_index(flags, 'P') >= 0)
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
