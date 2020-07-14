#include "../inc/header.h"

static char add_white(char c) {
    switch (c) {
        case 'n':
            return '\n';
        case 'r':
            return '\r';
        case 'f':
            return '\f';
        case 'a':
            return '\a';
        case 'b':
            return '\b';
        case 't':
            return '\t';
        case 'v':
            return '\v';
        default:
            return 0;
    }
}

static char *echo_parse(char *str, char *s, int *i) {
    char *rez = mx_strdup(str);

    if (s[*i + 1] == '0' && s[*i + 2] == '3' && s[*i + 3] == '3') {
        rez = mx_add(rez, '\033', i);
        *i += 3;
    }
    else if (s[*i + 1] == '\\') {
        *i += 1;
        rez = mx_add(rez, s[*i], i);
    }
    else if (add_white(s[*i + 1])) {
        *i += 1;
        rez = mx_add(rez, add_white(s[*i]), i);
    }
    else 
        rez = mx_add(rez, s[*i], i);
    free_mass(str, NULL, NULL, NULL);
    return rez;
}

char *mx_cmnd_echo_trans(char *src) {
    char *rez = NULL;

    for (int i = 0; src[i]; ) {
        if (src[i] == '\\')
            rez = echo_parse(rez, src, &i);
        else
            rez = mx_add(rez, src[i], &i);
    }
    if (src)
        free(src);
    return rez;
}
