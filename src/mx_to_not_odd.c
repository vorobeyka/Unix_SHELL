#include "../inc/header.h"

static char *parse_env(char *s, int *i, t_ost *tost) {
    char *rez = NULL;
    char *tmp = NULL;

    if (s[*i] == '{')
        tmp = mx_add_with_hook(s, i, tost);
    else
        tmp = mx_add_with_laters(s, i);
    if (tmp && getenv(tmp))
        rez = mx_strdup(getenv(tmp));
    mx_free_mass(tmp, NULL, NULL, NULL);
    return rez;
}

char *mx_parse_baks(char *s, int *i, t_ost *tost) {
    *i += 1;
    if (s[*i]
        && (mx_check_delims(s[*i]) == 3
        || mx_check_delims(s[*i]) == 1)) {
        return parse_env(s, i, tost);
    }
    return mx_strdup("$");
}

char *mx_split_quotes_x2(char *add, char *str, int *i, int *err) {
    char *rez = add ? mx_strdup(add) : NULL;

    rez = mx_add(rez, str[*i], i);
    for ( ; str[*i] && str[*i] != ')'; ) {
        rez = str[*i] == '$' && str[*i + 1] == '(' ? mx_split_quotes_x2(rez, str, i, err)
            : mx_add(rez, str[*i], i);
    }
    if (!str[*i]) {
        mx_printerr("Odd number of quotes.\n");
        mx_free_mass(rez, add, NULL, NULL);
        *err = 1;
        return NULL;
    }
    rez = mx_add(rez, str[*i], i);
    mx_free_mass(add, NULL, NULL, NULL);
    return rez;
}

char *mx_parse_odd(char *s, int *i) {
    char *rez = NULL;

    while (s[*i] && s[*i] != '\'')
        rez = mx_add(rez, s[*i], i);
    *i += 1;
    return rez;
}

char *mx_to_not_odd(char *rez, char *s, int *i, t_ost *tost) {
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
