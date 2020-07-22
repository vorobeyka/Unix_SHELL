#include "../inc/header.h"

static bool flags_check(char *str) {
    str++;
    for ( ; *str; str++)
        if (*str != 'n' && *str != 'e' && *str != 'E')
            return true;
    return false;
}

static char **check_flags(char **src, int *fl) {
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

static char **array_transform(char **src, int *flags) {
    char **rez = mx_array_copy_strs(src);

    for (int i = 0; rez[i]; i++)
        rez[i] = flags[1] ? mx_cmnd_echo_trans(rez[i])
               : mx_cmnd_echo_trans_x2(rez[i]);
    return rez;
}

static int mx_cmnd_echo(char **src, t_ost *tost) {
    int *flags = mx_create_integer_massive(2);
    char **s = array_transform(check_flags(src, flags), flags);
    int last = 0;

    for (char **str = s; str && *str; str++) {
        mx_printstr(*str);
        if (*(str + 1)) {
            mx_printchar(' ');
        }
        else if (flags[0]
                 && !tost->kostil_x2
                 && !mx_is_one_symb(mx_last_char(*str), *str, &last)) {
            tost->kostil = true;
        }
    }
    if (!flags[0])
        mx_printstr("\n");
    mx_del_strarr(&s);
    free(flags);
    return 0;
}

int mx_echo(char **argv, t_ost *tost) {
    if (tost->env)
        argv++;
    return mx_cmnd_echo(argv, tost);
}
