#include "../inc/header.h"

int mx_true(char **argv, t_ost *tost) {
    if (tost->env)
        printf("%s\n", argv[0]);
    return 0;
}

int mx_false(char **argv, t_ost *tost) {
    if (tost->env)
        printf("%s\n", argv[0]);
    return 1;
}

int mx_jobs(char **argv, t_ost *tost) {
    if (mx_lenn_mass(argv) > 1) {
        mx_printerr("jobs: too many arguments\n");
        return 1;
    }
    if (tost->jobs) {
        for (t_jobs *i = tost->jobs; i; i = i->next) {
            mx_printstr("[");
            mx_printint(i->num);
            mx_printstr("]");
            mx_printstr("  ");
            mx_printint(i->pid);
            mx_printstr("  ");
            mx_printchar(i->flag);
            mx_long_print(" ", "suspended", "  ", i->name);
            mx_printchar('\n');
        }
    }
    return 0;
}

int mx_change(char **argv, t_ost *tost) {
    if (mx_lenn_mass(argv) != 2) {
        mx_printerr("change [useful / user / fun / normal]\n");
        return 1;
    }
    if (mx_strcmp(argv[1], "normal") == 0)
        tost->flag = 0;
    else if (mx_strcmp(argv[1], "useful") == 0)
        tost->flag = 1;
    else if (mx_strcmp(argv[1], "user") == 0)
        tost->flag = 2;
    else if (mx_strcmp(argv[1], "fun") == 0)
        tost->flag = 3;
    else if (mx_strcmp(argv[1], "secret") == 0)
        tost->flag = 4;
    else {
        mx_long_error_print("there is not any parameters such as: ",
                            argv[1], "\n", NULL);
        return 1;
    }
    return 0;
}

int mx_next(char **argv, t_ost *tost) {
    if (mx_lenn_mass(argv) != 1) {
        mx_printerr("bro, just write: \"next\"\n");
        return 1;
    }
    else if (tost->flag != 3) {
        mx_printerr("you have to use \"change fun\" command before\n");
        return 1;    
    }
    tost->prompt = tost->prompt->next;
    return 0;
}

