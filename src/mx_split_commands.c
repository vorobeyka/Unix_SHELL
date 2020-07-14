#include "../inc/header.h"

static char *to_add(char *add, char *str, int *i, int *err) {
    int token = mx_check_delims(str[*i]);

    add = str[*i] == '\\' ? mx_parse_slash(add, str, i, 0)
          : !token ? mx_split_quotes(add, str, err, i)
          : token < 3 ? mx_split_hooks_baks(add, str, i, err)
          : mx_add(add, str[*i], i);
    return add;
}

char *mx_parse_command(char *str, char *s, int *i, t_ost *tost) {
    char *rez = mx_strdup(str);
    char *command = NULL;
    char *rezult = NULL;
    char **arr = NULL;

    *i += 1;
    free_mass(str, NULL, NULL, NULL);
    while (s && s[*i] && s[*i] != '`')
        command = mx_add(command, s[*i], i);
    if (s[*i] == '`')
        *i += 1;
    arr = mx_parse_all(command, tost);
    if (arr) {
        rezult = mx_prog_out(arr, tost);
        mx_del_strarr(&arr);
    }
    free(command);
    return mx_super_join(rez, rezult, 3);
}

char *mx_parse_command_x2(char *str, char *s, int *i, t_ost *tost) {
    char *rez = mx_strdup(str);
    char *command = NULL;
    char *rezult = NULL;
    char **arr = NULL;

    *i += 1;
    if (str)
        free(str);
    while (s && s[*i] && s[*i] != ')')
        command = mx_add(command, s[*i], i);
    if (s[*i] == ')')
        *i += 1;
    arr = mx_parse_all(command, tost);
    if (arr) {
        rezult = mx_prog_out(arr, tost);
        mx_del_strarr(&arr);
    }
    free(command);
    return mx_super_join(rez, rezult, 3);
}

char **mx_split_commands(char *str, t_ost *tost) {
    char **rez = NULL;
    char *add = NULL;

    for (int i = 0; str && str[i] != '\0'; ) {
        mx_shift_spaces(str, &i);
        add = to_add(add, str, &i, &tost->error);
        if (tost->error) {
            free_mass(add, NULL, NULL, NULL);
            mx_del_strarr(&rez);
            return NULL;
        }
        if (!str[i] || str[i] == ' ') {
            rez = mx_resize_one(rez, add);
            free_mass(add, NULL, NULL, NULL);
            add = NULL;
        }
    }
    return rez;
}
