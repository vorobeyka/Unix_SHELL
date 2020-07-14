#include "../inc/header.h"

static int do_builtin(char *builtin[],
                      char **argv,
                      int (*builtin_func[]) (char **, t_ost *tost),
                      t_ost *tost) {
    int result = 0;

    for (int i = 0; builtin[i]; i++) {
        if (mx_strcmp(argv[0], builtin[i]) == 0)
            result = (*builtin_func[i]) (argv, tost);
    }
    return result;
}

int mx_builtin(char **argv, t_ost *tost) {
    if (tost->error) {
        tost->error = 0;
        return 1;
    }
    char *builtin[] = {"env", "cd", "pwd", "which", "echo", "export",
                       "unset", "fg", "exit", "true", "false", "jobs",
                       "kill", "change", "next", "chdir", NULL};
    int (*builtin_func[]) (char **, t_ost *tost) = {&mx_env, &mx_cd,
                                                    &mx_pwd, &mx_which,
                                                    &mx_echo, &mx_export,
                                                    &mx_unset, &mx_fg,
                                                    &mx_exit, &mx_true,
                                                    &mx_false, &mx_jobs,
                                                    &mx_kill, &mx_change,
                                                    &mx_next, &mx_cd};
    
    return do_builtin(builtin, argv, builtin_func, tost);
}

bool mx_check_builtin(char *name) {
    char *builtin[] = {"env", "cd", "pwd", "which", "echo", "export",
                       "unset", "fg", "exit", "true", "false", "jobs",
                       "kill", "change", "next", "chdir", NULL};

    for (int i = 0; builtin[i]; i++)
        if (mx_strcmp(builtin[i], name) == 0)
            return true;
    return false;
}

static void exit_part(char **argv, t_ost *tost, int *result) {
    if (argv[1]) {
        *result = atoi(argv[1]);
        for (; *result > 255; *result -= 255)
            *result -= 1;
        for (; *result < 0; *result += 255)
            *result += 1;
    }
    else
        *result = tost->status;
    if (mx_cech_end())
        mx_printstr("\n\n[Process completed]");
    if (tost->exit_slesh == 1 && isatty(0))
        mx_printstr("\n");
}

int mx_exit(char **argv, t_ost *tost) {
    if (mx_lenn_mass(argv) > 2) {
        mx_printerr("exit: too many arguments\n");
        return 1;
    }
    if (tost->out == 1) {
        mx_printerr("\nush: you have suspended jobs.\n");
        tost->out = 0;
        return 0;
    }
    int result = 0;

    exit_part(argv, tost, &result);
    exit(result);
    return 0;
}
