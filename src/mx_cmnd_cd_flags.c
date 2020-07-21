#include "../inc/header.h"

static int (*cd_switch[3]) (t_built *, char *) = {
    &mx_cmnd_cd_with_s,
    &mx_cmnd_cd_with_p,
    &mx_cmnd_do_cd
};

static void print_with_home(char *ctlg) {
    if (!getenv("HOME") && chdir(getenv("HOME"))) {
        mx_long_print(ctlg, "\n", NULL, NULL);
        return;
    }
    if (getenv("HOME")[0] != '/') {
        mx_long_print(ctlg, "\n", NULL, NULL);
        return;
    }
    char *to_print = mx_replace_one_substr(ctlg, getenv("HOME"), "~");

    mx_long_print(to_print, "\n", NULL, NULL);
    mx_free_mass(to_print, NULL, NULL, NULL);
}

static int second_step(t_built *u) {
    print_with_home(u->oldctlg);
    return mx_cmnd_do_cd(u, mx_strdup(u->oldctlg));
}

static int first_step(char *new_catalog, t_built *u) {
    free(u->commands[1]);
    u->commands[1] = NULL;
    if (!u->commands[2])
        new_catalog = mx_correct_ctlg(u, u->home_catalog);
    else {
        u->commands[1] = mx_strdup(u->commands[2]);
        free(u->commands[2]);
        u->commands[2] = NULL;
        if (!mx_strcmp(u->commands[1], "-")) {
            return second_step(u);
        }
        new_catalog = mx_correct_ctlg(u, u->commands[1]);
    }
    return mx_cmnd_do_cd(u, new_catalog);
}

int mx_cmnd_cd_flags(t_built *u) {
    char *new_catalog = NULL;
    int token = mx_flagscd(u->commands[1]);

    if (!mx_strcmp(u->commands[1], "--")) {
        return first_step(new_catalog, u);
    }
    if (mx_get_substr_index(u->commands[1], "--") == 0) {
        return mx_error_return("ush: unknown flags\n", 1);
    }
    if (!mx_strcmp(u->commands[1], "-") && !u->commands[2]) {
        return second_step(u);
    }
    new_catalog = mx_cmnd_cd_new_ctlg(u);
    for (int i = 0; i < 3; i++) {
        if (i == token) {
            return (*cd_switch[i])(u, new_catalog);
        }
    }
    return 1;
}
