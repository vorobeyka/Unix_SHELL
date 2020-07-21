#include "../inc/header.h"

static char *input_catalog(t_built *u) {
    if (u->commands[2])
        return u->commands[2];
    else
        return u->commands[1];
}

char *mx_to_ctlg(char *str) {
    if (mx_last_char(str) == '/' && mx_strlen(str) > 1) {
        char *rez = NULL;

        for (int i = 0; i < mx_strlen(str) - 1; )
            rez = mx_add(rez, str[i], &i);
        mx_free_mass(str, NULL, NULL, NULL);
        return rez;
    }
    return str;
}

static void check_link(t_cd *cd, char *ctlg) {
    char *tmp = mx_cmnd_cd_dots(ctlg);

    cd->tmp = (char *) malloc(512 * sizeof(char));
    if (!chdir(tmp)) {
        cd->link = mx_strdup(tmp);
        cd->new = mx_strdup(cd->link);
        cd->phys = getcwd(NULL, 0);
    }
    if (tmp)
        free(tmp);
}

static t_cd *push_cd(char *ctlg) {
    t_cd *rez = (t_cd *) malloc(1 * sizeof(t_cd));

    rez->for_oldpwd = getenv("PWD");
    rez->link = NULL;
    rez->new = NULL;
    rez->phys = NULL;
    check_link(rez, ctlg);
    return rez;
}

int mx_cmnd_do_cd(t_built *u, char *ctlg) {
    if (chdir(ctlg)) {
        char *error_message = "cd: No such file or directory: ";

        mx_print_long_err(error_message, input_catalog(u), "\n", NULL);
        mx_free_mass(ctlg, NULL, NULL, NULL);
        return 1;
    }
    u->cd = push_cd(ctlg);
    if (u->cd->for_oldpwd)
        setenv("OLDPWD", u->cd->for_oldpwd, 1);
    mx_free_mass(u->curctlg, u->lpwd, u->ppwd, u->oldctlg);
    u->oldctlg = mx_strdup(getenv("OLDPWD"));
    u->curctlg = mx_strdup(u->cd->new);
    setenv("PWD", u->curctlg, 1);
    u->lpwd = mx_strdup(u->cd->link);
    u->ppwd = mx_strdup(u->cd->phys);
    mx_free_mass(u->cd->link, u->cd->new, ctlg, u->cd->phys);
    mx_free_mass(u->cd->tmp, u->cd, NULL, NULL);
    return 0;
}
