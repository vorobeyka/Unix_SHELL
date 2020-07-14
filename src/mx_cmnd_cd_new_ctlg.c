#include "../inc/header.h"

static char *parse_ctlg(char *cd_str, char *str) {
    char *rez = NULL;

    rez = mx_super_join(cd_str, "/", 0);
    rez = mx_super_join(rez, str, 1);
    return rez;
}

static char *correct(t_built *u, char *str, char c) {
    switch (c) {
        case '\0':
            return mx_strdup(u->home_catalog);
        case '/':
            return parse_ctlg(u->home_catalog, &str[2]);
        case '-':
            if (str[2] == '/')
                return parse_ctlg(u->oldctlg, &str[3]);
            else
                return mx_strdup(str);
        case '+':
            if (str[2] == '/')
                return parse_ctlg(u->curctlg, &str[3]);
            else
                return mx_strdup(str);
        default:
            return parse_ctlg("/Users", &str[1]);
    }
}

char *mx_correct_ctlg(t_built *u, char *str) {
    char *rez = NULL;

    if (str[0] == '/') {
        u->full_path = 1;
        rez = mx_strdup(str);
    }
    else {
        if (str[0] == '~') {
            rez = correct(u, str, str[1]);
        }
        else {
            if (!mx_strcmp(str, "-"))
                rez = mx_strdup(u->oldctlg);
            else 
                rez = parse_ctlg(u->curctlg, str);
        }
        u->full_path = 0;
    }
    return rez;
}

char *mx_cmnd_cd_new_ctlg(t_built *u) {
    if (!u->commands[2])
        return mx_correct_ctlg(u, u->home_catalog);
    else
        return mx_correct_ctlg(u, u->commands[2]);
}
