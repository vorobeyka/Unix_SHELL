#include "../inc/header.h"

static void part_find_flag(char **arr, int *flag, int *result) {
    char *s = *arr;

    for (int i = 1; s[i]; i++) {
        if (s[i] == 'a') {
            if (*flag == 0)
                *flag = 1;
        }
        else if (s[i] == 's')
            *flag = *flag;
        else {
            mx_printerr("which: bat option: -");
            write(2, &s[i], 1);
            mx_printerr("\n");
            *flag = 3;
            *result = 1;
            break;
        }
    }
}

static char **find_flag(char **arr, int *flag, int *result) {
    for (; *arr && **arr == '-'; arr += 1) {
        if (mx_strcmp(*arr, "--") == 0)
            return arr += 1;
        part_find_flag(arr, flag, result);
    }
    return arr;
}

static void mx_wich_part(char **shift, t_ost *tost,
                         int flag, int *result) {
    int finder = -1;

    for (; shift && *shift; shift += 1) {
        if (finder == -1 || flag == 1)
            mx_find_support(*shift, flag, &finder, tost);
        if (finder == -1 || flag == 1)
            mx_find_built_in(*shift, flag, &finder);
        if (finder == -1 || flag == 1)
            mx_find_exe(*shift, flag, &finder);
        if (finder == -1 ) {
            if (flag != 2) {
                mx_printerr(*shift);
                mx_printerr(" not found\n");
            }
            *result = 1;
        }
        finder = -1;
    }
}

int mx_which(char **argv, t_ost *tost) {
    char **shift = argv;
    int flag = 0;
    int result = 0;

    shift += 1;
    shift = find_flag(shift, &flag, &result);
    if (result == 1)
        return result;
    mx_wich_part(shift, tost, flag, &result);
    return result;
}

