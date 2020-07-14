#include "../inc/header.h"

static bool mx_empty_env(void) {
    extern char **environ;

    if (environ[0] == NULL)
        return true;
    return false;
}

static void mx_ush(t_ost *tost) {
    mx_set_signals();
    mx_ush_loop(tost);
    mx_dell_full(tost->history);
}

int main() {
    t_ost tost;
    extern char **environ;
    struct passwd *pw = NULL;

    if (mx_empty_env()) {
        pw = getpwuid(getuid());
        setenv("HOME", pw->pw_dir, 1);
        setenv("LOGNAME", getlogin(), 1);
        setenv("SHLVL", "0", 1);
        setenv("PWD", getcwd(NULL, 0), 1);
        setenv("OLDPWD", getcwd(NULL, 0), 1);
    }
    mx_new_terminal(&tost);
    mx_ush(&tost);
    return 0;
}
