#include "../inc/header.h"

static void mx_ush(t_ost *tost) {
    mx_set_signals();
    mx_ush_loop(tost);
    mx_dell_full(tost->history);
}

static void put_environ() {
    char *to_pwd = getcwd(NULL, 0);
    char *to_new_pwd = NULL;

    if (getenv("PWD") && !chdir(getenv("PWD"))) {
        to_new_pwd = getcwd(NULL, 0);
        if (mx_strcmp(to_pwd, to_new_pwd)) {
            setenv("PWD", to_pwd, 1);
            chdir(to_pwd);
        }
        free(to_new_pwd);
        to_new_pwd = NULL;
    }
    else {
        setenv("PWD", to_pwd, 1);
    }
    mx_free_mass(to_pwd, to_new_pwd, NULL, NULL);
    setenv("OLDPWD", getenv("PWD"), 1);
}

static void put_empty_environ() {
    struct passwd *pw = NULL;
    char *pwd = getcwd(NULL, 0);

    pw = getpwuid(getuid());
    setenv("HOME", pw->pw_dir, 1);
    setenv("LOGNAME", getlogin(), 1);
    setenv("SHLVL", "0", 1);
    setenv("PWD", pwd, 1);
    setenv("OLDPWD", getenv("PWD"), 1);
    mx_free_mass(pwd, NULL, NULL, NULL);
}

int main() {
    t_ost tost;
    extern char **environ;

    if (!environ || environ[0] == NULL) {
        put_empty_environ();
    }
    put_environ();
    mx_new_terminal(&tost);
    mx_ush(&tost);
    return 0;
}
