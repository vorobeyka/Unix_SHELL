#include "../inc/header.h"

static char *to_add(char *add, char *str, int *i, int *err) {
    int token = mx_check_delims(str[*i]);

    add = str[*i] == '\\' ? mx_parse_slash(add, str, i)
          : !token ? mx_split_quotes(add, str, err, i)
          : token < 3 ? mx_split_hooks_baks(add, str, i, err)
          : mx_add(add, str[*i], i);
    return add;
}

static char *add_ods(char *str, char *s, int *i) {
     char c = s[*i];

    str = mx_add(str, s[*i], i);
    for ( ; s[*i] && s[*i] != c; ) {
        if (s[*i] == '\\') {
            str = mx_add(str, s[*i], i);
            str = mx_add(str, s[*i], i);
        }
        else {
            str = mx_add(str, s[*i], i);
        }
    }
    str = mx_add(str, s[*i], i);
    return str;
}

static char *add_to_command_x2(char *str, char *s, int *i, t_ost *tost) {
    if (s[*i] == '\\') {
        str = mx_add(str, s[*i], i);
        str = mx_add(str, s[*i], i);
        return str;
    }
    else if (mx_check_quote(s[*i]) > 0) {
        return add_ods(str, s, i);
    }
    else if (s[*i] == '$' && s[*i + 1] == '(') {
        *i += 1;
        return mx_parse_command_x2(str, s, i, tost);
    }
    else if (s[*i] == '`') {
        return mx_parse_command_x2(str, s, i, tost);
    }
    else {
        return mx_add(str, s[*i], i);
    }
}

char *mx_parse_command_x2(char *str, char *s, int *i, t_ost *tost) {
    char *command = NULL;
    char c = s[*i] == '(' ? ')' : '`';

    *i += 1;
    while (s && s[*i] && s[*i] != c)
        command = add_to_command_x2(command, s, i, tost);
    if (s[*i] == c)
        *i += 1;
    str = mx_super_join(str, mx_prog_out(command, tost), 3);
    tost->kostil_x2 = false;
    mx_free_mass(command, NULL, NULL, NULL);
    return str;
}

char **mx_split_commands(char *str, t_ost *tost) {
    char **rez = NULL;
    char *add = NULL;

    for (int i = 0; str && str[i] != '\0'; ) {
        mx_shift_spaces(str, &i);
        if (str[i] == '\0')
            break;
        add = to_add(add, str, &i, &tost->error);
        if (tost->error) {
            mx_free_mass(add, NULL, NULL, NULL);
            mx_del_strarr(&rez);
            return NULL;
        }
        if (!str[i] || str[i] == ' ') {
            rez = mx_resize_one(rez, add);
            mx_free_mass(add, NULL, NULL, NULL);
            add = NULL;
        }
    }
    return rez;
}
