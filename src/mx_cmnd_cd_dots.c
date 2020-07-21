#include "../inc/header.h"

static char *delete_dots(char *str) {
    char *rez = NULL;

    for (int i = 0; str[i]; ) {
        if (mx_get_substr_index(&str[i], "/./") == 0)
            i += 2;
        else
            rez = mx_add(rez, str[i], &i);
    }
    mx_free_mass(str, NULL, NULL, NULL);
    return rez;
}

static char *delete_one(char *rez, char *s, int i) {
    if (rez) {
        free(rez);
        rez = NULL;
    }
    if (!mx_strcmp(s, "/")) {
        return s;
    }
    if (s[i] == '/')
        i--;
    for ( ; s[i]; i--) {
        if (s[i] == '/')
            break;
    }
    for ( ; s[i]; i--) {
        if (s[i] == '/')
            break;
    }
    for (int j = 0; j < i; )
        rez = mx_add(rez, s[j], &j);
    return rez;
}

static char *dir_out(char *s) {
    char *rez = NULL;

    for (int i = 0; s[i]; ) {
        if (i == mx_strlen(s) - 1 && s[i] == '/' && mx_strlen(s) > 1)
            break;
        if (mx_get_substr_index(&s[i], "/../") == 0) {
            rez = delete_one(rez, s, i);
            i += 3;
            while (s[i])
                rez = mx_add(rez, s[i], &i);
            rez = dir_out(rez);
        }
        else {
            rez = mx_add(rez, s[i], &i);
        }
    }
    mx_free_mass(s, NULL, NULL, NULL);
    return rez;
}

char *clean_slash(char *str) {
    char *rez = NULL;

    for (int i = 0; str[i]; ) {
        if (str[i] == '/' && str[i + 1] == '/') {
            i += 1;
        }
        else {
            rez = mx_add(rez, str[i], &i);
        }
        if (str[i] == '/' && !str[i])
            break;
    }
    if (rez[mx_strlen(rez) - 1] != '/')
        rez = mx_super_join(rez, mx_strdup("/"), 3);
    if (rez[0] != '/')
        rez = mx_super_join(mx_strdup("/"), rez, 3);
    return rez;
}

char *mx_cmnd_cd_dots(char *ctlg) {
    char *rez = NULL;
    char *tmp = NULL;

    rez = delete_dots(clean_slash(ctlg));
    tmp = rez;
    rez = dir_out(clean_slash(rez));
    mx_free_mass(tmp, NULL, NULL, NULL);
    return rez;
}
