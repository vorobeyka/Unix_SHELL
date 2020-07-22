#include "../inc/header.h"

static char *parse_not_odd(char *s, int *i, t_ost *tost) {
    char *rez = NULL;

    while (s[*i] && s[*i] != '"') {
        if (s[*i] == '\\')
            rez = mx_parse_not_odd_slash(rez, s, i);
        else if (s[*i] == '$')
            rez = mx_to_not_odd(rez, s, i, tost);
        else if (s[*i] == '`')
            rez = mx_parse_command_x2(rez, s, i, tost);
        else
            rez = mx_add(rez, s[*i], i);
    }
    if (s[*i] == '"')
        *i += 1;
    return rez;
}

char *mx_add_quotes(char *str, char *s, int *i, t_ost *tost) {
    char *rez = NULL;

    *i += 1;
    if (s[*i - 1] == '\'') {
        rez = mx_super_join(str, mx_parse_odd(s, i), 3);
    }
    else {
        rez = mx_super_join(str, parse_not_odd(s, i, tost), 3);
    }
    return rez;
}

static char *first_open(char *rez, char *s, t_ost *tost, int *i) {
    if (s[*i + 1] == '?') {
        rez = mx_super_join(rez, mx_itoa(tost->status), 3);
        *i += 2;
    }
    else if (s[*i + 1] == '(') {
        *i += 1;
        rez = mx_parse_command_x2(rez, s, i, tost);
    }
    else {
        rez = mx_super_join(rez, mx_parse_baks(s, i, tost), 3);
    }
    return rez;
}

char *mx_open_quotes(char *s, t_ost *tost) {
    char *rez = NULL;

    for (int i = 0 ; s[i]; ) {
        if (s[i] == '\\') {
            i += 1;
            rez = mx_add(rez, s[i], &i);
        }
        else if (s[i] == '$')
            rez = first_open(rez, s, tost, &i);
        else if (s[i] == '`')
            rez = mx_parse_command_x2(rez, s, &i, tost);
        else {
            rez = mx_check_quote(s[i]) > 0 ?
                  mx_add_quotes(rez, s, &i, tost) :
                  mx_add(rez, s[i], &i);
        }
    }
    free(s);
    return rez;
}

char *mx_parse_slash(char *str, char *s, int *i) {
    str = mx_add(str, s[*i], i);
    str = mx_add(str, s[*i], i);
    return str;
}
