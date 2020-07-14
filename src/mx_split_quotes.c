#include "../inc/header.h"

static char *error_with_null(char *str, int *err) {
    mx_printerr("Odd number of quotes.\n");
    *err = 1;
    free_mass(str, NULL, NULL, NULL);
    return NULL;
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
        add = mx_add(add, str[*i], i);
    }
    if (!str[*i]) {
        return error_with_null(add, err);
    }
    add = mx_add(add, str[*i], i);
    return add;
}
