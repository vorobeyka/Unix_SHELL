#include "header.h"

bool check_char(char c) {
    if (c == 'P'
        || c == 's'
        || c == '-') {
        return true;
    }
    return false;
}

int flagscd(char *str) { 
    /*returns 0 if @'P'
    returns 1 if 's'
    returns 2 if nothing
    returns -1 if ilegal
    **/
    for (char *s = str; *s; s++)
        if (!check_char(*s)) {
            mx_printerr("cd: illegal flags\n");
            return -1;
        }
    if (mx_get_char_index(str, 'P') > mx_get_char_index(str, 's')) {
        return 1;
    }
    else if (mx_get_char_index(str, 'P') < mx_get_char_index(str, 's')) {
        return 0;
    } 
    else {
        return 2;
    }
}

char *parse_ctlg(char *cd_str, char *str) {
    char *rez = NULL;

    rez = mx_super_join(cd_str, "/", 0); //ЗАМЕНИТЬ НА ЕНВ ХОМ
    rez = mx_super_join(rez, str, 1);
    return rez;
}

char *correct_ctlg(t_built *u, char *str) {
    char *rez = NULL;

    if (str[0] == '/') {
        u->full_path = 1;
        rez = mx_strdup(str);
    }
    else {
        if (str[0] == '~') {
            switch (str[1]) {
                case '\0':
                    rez = mx_strdup(u->home_catalog);
                    break;
                case '/':
                    rez = parse_ctlg(u->home_catalog, &str[2]);
                    break;
                case '-':
                    if (str[2] == '/')
                        rez = parse_ctlg(u->oldctlg, &str[3]);
                    else
                        rez = mx_strdup(str);
                    break;
                case '+':
                    if (str[2] == '/')
                        rez = parse_ctlg(u->curctlg, &str[3]);
                    else
                        rez = mx_strdup(str);
                    break;
                default:
                    rez = parse_ctlg("/Users", &str[1]);
                    break;
            }
            
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

int cd(t_built *u, char *ctlg) {
    char *tmp = NULL;
    char *phys = NULL;
    char *link = NULL;
    char *new = NULL;
    char *for_oldpwd = getenv("PWD");

    if (chdir(ctlg)) {
        mx_print_long_err("cd: No such file or directory: ", ctlg, "\n", NULL);
        free(ctlg);
        return 1;
    }
    if (readlink(ctlg, tmp, 512) > 0) {
        link = mx_strdup(ctlg);
        new = mx_strdup(ctlg);
        phys = mx_strjoin(ctlg, mx_strjoin("/", tmp));
        chdir(phys);
        phys = getcwd(NULL, 0);
        chdir(ctlg);
        free(tmp);
    }
    else {
        phys = getcwd(NULL, 0);
        link = getcwd(NULL, 0);
        new = getcwd(NULL, 0);
    }
    if (for_oldpwd)
        setenv("OLDPWD", for_oldpwd, 1);
    u->oldctlg = getenv("OLDPWD");
    u->curctlg = mx_strdup(new);
    setenv("PWD", u->curctlg, 1);
    u->lpwd = mx_strdup(link);
    u->ppwd = mx_strdup(phys);
    free(phys);
    if (link)
        free(link);
    if (new)
        free(new);
    if (ctlg)
        free(ctlg);
    return 1;
}

int cd_with_s(t_built *u, char *ctlg) {
    char tmp[512] = { 0 };
    
    if (readlink(ctlg, tmp, 512) > 0) {
        mx_printerr("cd: No dir\n");
        return 1;
    }
    return cd(u, ctlg);
}

char *full_path(char *str) {
    char *rez = NULL;
    int len = mx_strlen(str);

    if (mx_reverse_index(str, '/') == 0 && len != 1)
        len--;
    for (int i = len - 1; str[i] != '/'; i--)
        len--;
    rez = (char *) malloc((len + 1) * sizeof(char));
    strncpy(rez, str, len);
    rez[len] = '\0';
    return rez;
}

int cd_with_p(t_built *u, char *ctlg) {
    char tmp[512] = { 0 }; 
    char *new_ctlg = NULL;
    
    if (readlink(ctlg, tmp, 512) > 0) {
        if (u->full_path)
            new_ctlg = full_path(u->commands[2]);
        else
            new_ctlg = mx_strjoin(u->curctlg, "/");
        free(ctlg);
        return cd(u, mx_super_join(new_ctlg, tmp, 1));
    }
    else {
        return cd(u, ctlg);
    }
}

int (*cd_switch[3]) (t_built *, char *) = {
    &cd_with_s,
    &cd_with_p,
    &cd
};

int mx_cd_flags(t_built *u) {
    char *new_catalog = NULL;

    if (!mx_strcmp(u->commands[1], "--")) {
        free(u->commands[1]);
        u->commands[1] = NULL;
        if (!u->commands[2])
            new_catalog = correct_ctlg(u, u->home_catalog);// ЗАМЕНИТЬ НА ENV(HOME) //А ТАК ЖЕ КОРРЕКТНО ДОПИЛИТЬ
        else {
            u->commands[1] = mx_strdup(u->commands[2]);
            free(u->commands[2]);
            u->commands[2] = NULL;
            if (!mx_strcmp(u->commands[1], "-")) {
                if (getenv("OLDPWD") && mx_strlen(getenv("OLDPWD"))) {
                    return cd(u, getenv("OLDPWD")/*u->oldctlg*/);
                }
                else {
                    mx_printerr("cd: old cat not set\n");
                    return 1;
                }
            }
            new_catalog = correct_ctlg(u, u->commands[1]);
        }
        return cd(u, new_catalog);
    }
    if (mx_get_substr_index(u->commands[1], "--") == 0) {
        mx_printerr("u$h: unknown flags\n");
        return 1;
    }
    if (!mx_strcmp(u->commands[1], "-") && !u->commands[2]) {
        if (getenv("OLDPWD") && mx_strlen(getenv("OLDPWD"))) {
            return cd(u, mx_strdup(getenv("OLDPWD"))/*u->oldctlg*/);
        }
        else {
            mx_printerr("cd: old cat not set\n");
            return 1;
        }
    }
    if (!u->commands[2])
        u->commands[2] = mx_strdup(u->home_catalog);
    new_catalog = correct_ctlg(u, u->commands[2]);
    for (int i = 0; i < 3; i++)
        if (i == flagscd(u->commands[1])) {
            return (*cd_switch[i])(u, new_catalog);
        }
    return 0;
}

int mx_cmndcd(t_built *u) {
    int i = mx_len_of_array(u->commands);
    if (i > 3
        || (u->commands[1] && u->commands[1][0] != '-' && i == 3)) {
        mx_printerr("u$h: cd: too many arguments\n");
        return 1;
    }
    if (u->commands[1] && u->commands[1][0] == '-') {
        return mx_cd_flags(u);
    }
    else {
        if (!u->commands[1])
            u->commands[1] = mx_strdup(u->home_catalog);
        return cd(u, correct_ctlg(u, u->commands[1]));
    }
}

int mx_cd(char **argv, t_ost *tost) {
    tost->built->commands = argv;
    return mx_cmndcd(tost->built);
}
