#include "../inc/header.h"

void mx_shift_spaces(char *str, int *i) {
    if (str && str[*i] == ' ') {
        *i += 1;
        mx_shift_spaces(str, i);
    }
}

void mx_find_delim(char *str, char delim, int *i) {
    if (str[*i] && str[*i] != delim) {
        *i += 1;
        mx_find_delim(str, delim, i);
    }
}

void mx_last_delim(char *str, char delim, int *i) {
    if (str[*i] && str[*i] == delim) {
        *i += 1;
        mx_last_delim(++str, delim, i);
    }
}

int mx_check_delims(char c) {
    if (c == '\'' || c == '"' || c == '`') {
        return 0;
    }
    else if (c == '{') {
        return 1;
    }
    else if (c == '$') {
        return 2;
    }
    return 3;
}
