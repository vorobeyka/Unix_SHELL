#include "../inc/header.h"

bool mx_is_space(char c) {
    switch (c) {
        case '\a':
            return true;
        case '\r':
            return true;
        case '\b':
            return true;
        default:
            return false;
    }
}

bool mx_is_last_char(char c) {
    switch (c)
    {
        case '\n':
            return true;
        case '\v':
            return true;
        default:
            return false;
    }
}

bool mx_is_one_symb(char c, char *str, int *flag) {
    if (mx_strlen(str) == 1 && (mx_is_last_char(c) || mx_is_space(c))){
        *flag = 1;
        return true;
    }
    else if (c == '\n') {
        *flag = 1;
        return true;
    }
    
    return false;
}

char mx_last_char(char *str) {
    if (str) {
        while (str && *str)
            str++;
        str--;
    }
    return *str;
}
