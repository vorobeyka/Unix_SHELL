#include "../inc/header.h"

static char *create_buff(void) {
    char *s = NULL;
    char ch;

    while (read(0, &ch, 1) > 0)
        s = mx_join_char(s, ch);
    s[mx_strlen(s) - 1] = '\0';
    return s;
}

static void shell_level(void) {
    char *old = getenv("SHLVL");
    char *new = NULL;
    
    if (old)
        new = mx_itoa(atoi(old) + 1);
    else
        new = mx_itoa(1);
    if (setenv("SHLVL", new, 1) == -1) {
        free(new);
        perror("ush: setenv");
        exit(1);
    }
    free(new);
}

t_built *push_built() {
    t_built *rez = (t_built *) malloc(1 * sizeof(t_built));
    struct passwd *pw = getpwuid(getuid());

    rez->cd = NULL;
    rez->home_catalog = pw->pw_dir;
    rez->command_line = NULL;
    rez->commands = NULL;
    rez->curctlg = mx_strdup(getenv("PWD"));
    rez->lpwd = NULL;
    rez->ppwd = getcwd(NULL, 0);
    if (chdir(rez->curctlg))
        rez->curctlg = getcwd(NULL, 0);
    rez->old_cat = 0;
    if (!chdir(getenv("OLDPWD")))
        rez->oldctlg = mx_strdup(getenv("OLDPWD"));
    else
        rez->oldctlg = mx_strdup(rez->curctlg);
    chdir(rez->curctlg);
    rez->status = 1;
    return rez;
}

static void for_tost(t_ost **tost) {
    extern char **environ;

    (*tost)->kostil = false;
    (*tost)->kostil_x2 = false;
    (*tost)->built = push_built();
    (*tost)->error = 0;
    (*tost)->env = environ;
    (*tost)->out = 0;
    (*tost)->status = 0;
    (*tost)->flag = 0;
    (*tost)->exit_slesh = 0;
    (*tost)->path = getenv("_");
    (*tost)->jobs = NULL;
    (*tost)->history = NULL;
    (*tost)->support = NULL;
    (*tost)->prompt = NULL;
    (*tost)->redi = NULL;
}

void mx_new_terminal(t_ost *tost) {
    for_tost(&tost);
    mx_feel_it(&tost->prompt);
    shell_level();
    if (isatty(0)) {
        tcgetattr(0, &tost->old);
        tcgetattr(0, &tost->read);
        tost->read.c_lflag &= ~(ICANON | ECHO | ISIG);
        tost->read.c_cc[VMIN] = 1;
    }
    else {
        char *s = create_buff();
        int result = mx_start(tost, s);
        
        free(s);
        exit(result);
    }
}
