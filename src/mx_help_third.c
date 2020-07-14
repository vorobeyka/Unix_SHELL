#include "../inc/header.h"

char *mx_join_char(char *src, char c) {
    char *s = mx_strnew(1);

    s[0] = c;
    src = mx_super_join(src, s, 1);
    free(s);
    return src;
}

bool mx_cech_end(void) {
    char *s = getenv("SHLVL");
    int num = atoi(s);

    if (num == 1)
        return true;
    return false;
}

int mx_reverse_index(char *src, char s) {
    int index = -1;

    if (!src)
        return index;
    for (int i = mx_strlen(src) - 1; i >= 0; i--)
        if (src[i] == s)
            index = i;
    return index;
}

char *mx_super_join(char *src1, char *src2, int fl) {
    char *s = mx_strjoin(src1, src2);

    if (fl == 1) {
        free(src1);
        src1 = NULL;
    }
    else if (fl == 2) {
        free(src2);
        src2 = NULL;
    }
    else if (fl == 3) {
        free(src1);
        free(src2);
        src1 = NULL;
        src2 = NULL;
    }
    return s;
}

void mx_long_print(char *src1, char *src2, char *src3, char *src4) {
    if (src1)
        mx_printstr(src1);
    if (src2)
        mx_printstr(src2);
    if (src3)
        mx_printstr(src3);
    if (src4)
        mx_printstr(src4);
}
