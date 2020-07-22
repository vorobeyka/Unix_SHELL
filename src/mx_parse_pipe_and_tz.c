#include "../inc/header.h"

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
    case '`':
        return '`';
    case '\\':
        return '\\';
    default:
        return 0;
    }
}

char *next_shift_one(char *add, char *s, char c, int *i) {
    if (c == '}' && mx_get_char_index(s, '}') < 0) {
        while (s[*i] && s[*i] != ';' && s[*i] != '|') {
            add = to_shift(s[*i]) ? mx_shift_one(add, s, to_shift(s[*i]), i)
                : mx_add(add, s[*i], i);
        }
    }
    else {
        while (s[*i] && s[*i] != c) {
            add = to_shift(s[*i]) ? mx_shift_one(add, s, to_shift(s[*i]), i)
                : mx_add(add, s[*i], i);
        }
    }
    return add;
}

char *mx_shift_one(char *add, char *s, char c, int *i) {
    if (c == '\\') {
        add = mx_add(add, s[*i], i);
        if (s[*i])
            add = mx_add(add, s[*i], i);
        return add;
    }
    add = mx_add(add, s[*i], i);
    add = next_shift_one(add, s, c, i);
    
    if (s[*i] == c)
        add = mx_add(add, s[*i], i);
    return add;
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
            mx_free_mass(add, NULL, NULL, NULL);
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
    char *new_line = mx_cut_subsh(line);

    if (new_line) {
        rez = mx_split_comm(new_line, c);
    }
    mx_free_mass(new_line, NULL, NULL, NULL);
    return rez;
}
