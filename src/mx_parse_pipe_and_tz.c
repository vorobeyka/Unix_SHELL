#include "../inc/header.h"

char *mx_shift_one(char *add, char *s, char c, int *i) {
    for (int j = 0; c == '\\' && j < 2; j++)
        add = mx_add(add, s[*i], i);
    if (c == '"' || c == '\'') {
        add = mx_add(add, s[*i], i);
    }
    if (c == '}' && mx_get_char_index(s, '}') < 0) {
        while (s[*i] && s[*i] != ';' && s[*i] != '|')
            add = mx_add(add, s[*i], i);
    }
    else {
        while (s[*i] && s[*i] != c)
            add = mx_add(add, s[*i], i);
    }
    if (s[*i] == c)
        add = mx_add(add, s[*i], i);
    return add;
}

char to_shift(char c) {
    switch (c) {
    case '{':
        return '}';
    case '(':
        return ')';
    case '"':
        return '"';
    case '\'':
        return '\'';
    case '\\':
        return '\\';
    default:
        return 0;
    }
}

char **mx_split_comm(char *s, char delim) {
    char **rez = NULL;
    char *add = NULL;

    for (int i = 0; s && s[i] != '\0'; ) {
        add = to_shift(s[i]) ? mx_shift_one(add, s, to_shift(s[i]), &i)
            : s[i] != delim ? mx_add(add, s[i], &i)
            : add;
        if (!s[i] || s[i] == delim) {
            rez = mx_resize_one(rez, add);
            free_mass(add, NULL, NULL, NULL);
            add = NULL;
            if (s[i] == delim) {
                i++;
                continue;
            }
        }
    }
    return rez;
}

char **mx_parse_pipes_and_tz(char *line, char c) {
    char **rez = NULL;

    if (line) {
        rez = mx_split_comm(line, c);
    }
    return rez;
}