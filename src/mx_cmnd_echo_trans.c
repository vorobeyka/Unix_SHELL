#include "../inc/header.h"

char mx_add_white(char c) {
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

static char is_white(char c) {
    switch (c) {
        case '\n':
            return 'n';
        case '\r':
            return 'r';
        case '\f':
            return 'f';
        case '\a':
            return 'a';
        case '\b':
            return 'b';
        case '\t':
            return 't';
        case '\v':
            return 'v';
        default:
            return 0;
    }
}

char *mx_cmnd_echo_trans_x2(char *src) {
    char *rez = NULL;

    for (int i = 0; src[i]; ) {
        if (src[i] =='\\') {
            if (src[i + 1] == '\\') {
                i++;
                rez = mx_add(rez, src[i], &i);
            }
            else if (mx_add_white(src[i + 1])) {
                i++;
                rez = mx_add(rez, mx_add_white(src[i]), &i);
            }
            else
                rez = mx_add(rez, src[i], &i);
        }
        else
            rez = mx_add(rez, src[i], &i);
    }
    mx_free_mass(src, NULL, NULL, NULL);
    return rez;
}

char *mx_cmnd_echo_trans(char *src) {
    char *rez = NULL;

    for (int i = 0; src[i]; ) {
        if (is_white(src[i])) {
            rez = mx_add(rez, '\\', &i);
            i--;
            rez = mx_add(rez, is_white(src[i]), &i);
        }
        else
            rez = mx_add(rez, src[i], &i);
    }
    if (src)
        free(src);
    return rez;
}
