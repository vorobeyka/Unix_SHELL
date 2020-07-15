#include "../inc/header.h"

bool check_char(char c) {
    if (c == 'P'
        || c == 's'
        || c == '-') {
        return true;
    }
    return false;
}

int mx_flagscd(char *str) {
    for (char *s = str; *s; s++) {
        if (!check_char(*s)) {
            return mx_error_return("cd: illegal flags\n", -1);
        }
    }
    if (mx_get_char_index(str, 's') >= 0) {
        return 0;
    }
    if (mx_get_char_index(str, 'P') >= 0) {
        return 1;
    }
    return 2;
}

int mx_cmnd_cd(t_built *u) {
    int i = mx_len_of_array(u->commands);

    if (i > 3 || (u->commands[1] && u->commands[1][0] != '-' && i == 3)) {
        mx_printerr("u$h: cd: too many arguments\n");
        return 1;
    }
    if (u->commands[1] && u->commands[1][0] == '-')
        return mx_cmnd_cd_flags(u);
    else {
        if (!u->commands[1])
            return mx_cmnd_do_cd(u, mx_correct_ctlg(u, u->home_catalog));
        else
            return mx_cmnd_do_cd(u, mx_correct_ctlg(u, u->commands[1]));
    }
}

int mx_cd(char **argv, t_ost *tost) {
    int result = 0;

    tost->built->commands = argv;
    result = mx_cmnd_cd(tost->built);
    return result;
}
