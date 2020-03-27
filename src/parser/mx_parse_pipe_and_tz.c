#include "header.h"

char *mx_shift_one(char *add, char *s, char c, int *i) {
    char *rez = mx_strdup(add);

    if (c == '"' || c == '\'') {
        rez = mx_add(rez, s[*i], i);
        if (mx_get_char_index(&s[*i], c) < 0) {
            mx_printerr("Odd quote\n");
            return NULL;
        }
    }
    if (c == '}' && mx_get_char_index(s, '}') < 0)
        while (s[*i] && s[*i] != ';' && s[*i] != '|')
            rez = mx_add(rez, s[*i], i);
    else
        while (s[*i] && s[*i] != c)
            rez = mx_add(rez, s[*i], i);
    
    if (c == '"' || c == '\'')
        rez = mx_add(rez, s[*i], i);
    if (add)
        free(add);
    return rez;
}

char check_to_shift(char c) {
    switch (c) {
    case '{':
        return '}';
        break;
    case '(':
        return ')';
    case '"':
        return '"';
    case '\'':
        return '\'';
    default:
        return 0;
        break;
    }
}

char **mx_split_comm(char *s, char delim) {
    char **rez = NULL;
    char *add = NULL;
    // int token;

    for (int i = 0; s && s[i] != '\0'; ) {
        char c = check_to_shift(s[i]);

        add = c ? mx_shift_one(add, s, c, &i)
            : s[i] != delim ? mx_add(add, s[i], &i)
            : add;
        if (!s[i] || s[i] == delim) {
            rez = mx_resize_one(rez, add);
            if (add)
                free(add);
            add = NULL;
            if (s[i] == delim) {
                i++;
                continue;
            }
        }
    }
    // if (s)
        // free(s);
    return rez;
}

char **mx_parse_pipes_and_tz(char *line, char c) {
    char **rez = NULL;

    if (line) {
        rez = mx_split_comm(line, c);
        // for (int i = 0; rez && rez[i]; i++) {
            // rez[i]  = parse_delim(rez[i]);
        // }
    }
    // mx_printint(mx_len_of_array(rez));
    // for (int i = 0; rez[i]; i++) {
        // mx_printstr(rez[i]);
    // }
    // exit(1);
    return rez;
}
