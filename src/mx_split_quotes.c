#include "../inc/header.h"

static bool check_slash_parse(char c) {
    if (c == '\"' || c == '`') {
        return true;
    }
    return false;
}

char *mx_parse_not_odd_slash(char *add, char *s, int *i) {
    *i += 1;
    if (mx_add_white(s[*i])) {
        add = mx_add(add, mx_add_white(s[*i]), i);
    }
    else if (s[*i] == '\\') {
        add = mx_add(add, s[*i], i);
    }
    else if (s[*i] == '$') {
        add = mx_add(add, s[*i], i);
    }
    else if (check_slash_parse(s[*i])) {
        add = mx_add(add, s[*i], i);
    }
    else {
        *i -= 1;
        add = mx_add(add, s[*i], i);
        add = mx_add(add, s[*i], i);
    }
    return add;
}

static char *error_with_null(char *str, int *err) {
    mx_printerr("Odd number of quotes.\n");
    *err = 1;
    mx_free_mass(str, NULL, NULL, NULL);
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
    if (str[*i] != '\0')
        add = mx_add(add, str[*i], i);
    return add;
}

char *mx_split_quotes(char *add, char *str, int *err, int *i) {
    char c = str[*i];

    add = mx_add(add, str[*i], i);
    for ( ; str[*i] && str[*i] != c; ) {
        if (str[*i] == '$' && str[*i + 1] == '(' && (c == '"' || c == '`')) {
            add = mx_split_quotes_x2(add, str, i, err);
            if (*err) {
                mx_free_mass(add, NULL, NULL, NULL);
                return NULL;
            }
            continue;
        }
        else
            add = add_with_slash(add, str, c, i);
    }
    if (!str[*i]) {
        return error_with_null(add, err);
    }
    add = mx_add(add, str[*i], i);
    return add;
}
