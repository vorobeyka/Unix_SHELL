#include "../inc/header.h"

static bool check_slash_parse(char c) {
    if (c == '\''
        || c == '('
        || c == ')'
        || c == '{'
        || c == '}') {
        return true;
    }
    return false;
}

char *mx_parse_not_odd_slash(char *add, char *s, int *i) {
    if (check_slash_parse(s[*i + 1])) {
        add = mx_add(add, s[*i], i);
        add = mx_add(add, s[*i], i);
    }
    else {
        *i += 1;
        add = mx_add(add, s[*i], i);
    }
    return add;
}

static char *error_with_null(char *str, int *err) {
    mx_printerr("Odd number of quotes.\n");
    *err = 1;
    free_mass(str, NULL, NULL, NULL);
    return NULL;
}

char *add_with_slash(char *add, char *str, char c, int *i) {
    if (str[*i] != '\\') {
        return mx_add(add, str[*i], i);
    }
    if (c == '\'') {
        return mx_add(add, str[*i], i);
    }
    add = mx_add(add, str[*i], i);
    add = mx_add(add, str[*i], i);
    return add;
}

char *mx_split_quotes(char *add, char *str, int *err, int *i) {
    char c = str[*i];

    add = mx_add(add, str[*i], i);
    for ( ; str[*i] && str[*i] != c; ) {
        if (str[*i] == '$' && str[*i + 1] == '(' && c == '"') {
            add = mx_split_quotes_x2(add, str, i, err);
            if (*err) {
                free_mass(add, NULL, NULL, NULL);
                return NULL;
            }
            continue;
        }
        add = add_with_slash(add, str, c, i);
    }
    if (!str[*i]) {
        return error_with_null(add, err);
    }
    add = mx_add(add, str[*i], i);
    return add;
}
