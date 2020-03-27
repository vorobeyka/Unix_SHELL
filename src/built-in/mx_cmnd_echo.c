#include "header.h"

bool flags_check(char *str) {
    str++;
    for ( ; *str; str++) {
        if (*str != 'n' && *str != 'e' && *str != 'E')
            return true;
    }
    return false;
}

static char **check_flags(char **src, int *fl) {
    for (int i = 0; i < 2; i++)
        fl[i] = 0;
    while (src && *src && *src[0] == '-') {
        if (flags_check(*src))
            break;
        for (char *s = *src; *s; s++) {
            if (*s == 'n')
                fl[0] = 1;
            if (*s == 'E')
                fl[1] = 1;
            if (*s == 'e')
                fl[1] = 0;
        }
        char *tmp = *src;

        src++;
        if (!tmp[1])
            break;
    }
    return src;
}

char **mx_arraydup(char **src) {
    int len = mx_len_of_array(src);
    char **rez = (char **) malloc((len + 1) * sizeof(char *));

    for (int i = 0; src[i]; i++)
        rez[i] = mx_strdup(src[i]);
    rez[len] = NULL;
    return rez;
}

char add_white(char c) {
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
        break;
    }
}

char *parse_escape(char *str, char *s, int *i) {
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
    else {
        rez = mx_add(rez, s[*i], i);
    }
    if (str)
        free(str);
    return rez;
}

char *trans_e(char *src) {
    char *rez = NULL;

    for (int i = 0; src[i]; ) {
        if (src[i] == '\\')
            rez = parse_escape(rez, src, &i);
        else
            rez = mx_add(rez, src[i], &i);
    }
    if (src)
        free(src);
    return rez;
}

char **array_transform(char **src, int *flags) {
    char **rez = mx_arraydup(src);

    for (int i = 0; rez[i]; i++)
        rez[i] = !flags[1] ? trans_e(rez[i]) : rez[i];
    return rez;
}

int mx_cmnd_echo(char **src) {
    int *flags = (int *) malloc(2 * sizeof(int));
    char **s = array_transform(check_flags(src, flags), flags);

    for (char **str = s; str && *str; str++) {
        mx_printstr(*str);
        if (*(str + 1))
            mx_printchar(' ');
        else if (!flags[0])
            mx_printstr("\n");
    }
    mx_del_strarr(&s);
    free(flags);
    // if mx_printchar('\n');
    return 0;
}

int mx_echo(char **argv, t_ost *tost) {
    if (tost->env) {
        argv++;
    }
    return mx_cmnd_echo(argv);
}
