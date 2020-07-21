#include "../inc/header.h"

char **mx_resize_array(char **s, char **add) {
    if (!add || !*add)
        return s;
    char **rez = NULL;

    for (char **array = s; array && *array; array++)
        rez = mx_resize_one(rez, *array);
    mx_del_strarr(&s);
    for (char **array = add; array && *array; array++)
        rez = mx_resize_one(rez, *array);
    mx_del_strarr(&add);
    return rez;
}

char *mx_split_hooks_baks(char *add, char *str, int *i, int *err) {
    char *rez = add ? mx_strdup(add) : NULL;

    rez = mx_add(rez, str[*i], i);
    for ( ; str[*i] && str[*i] != ' '; ) {
        rez = str[*i] == '\'' || str[*i] == '"' || str[*i] == '`'
            ? mx_split_quotes(rez, str, err, i)
            : str[*i] == '(' ? mx_split_quotes_x2(rez, str, i, err)
            : mx_add(rez, str[*i], i);
        if (*err) {
            mx_free_mass(add, rez, NULL, NULL);
            return NULL;
        }
    }
    mx_free_mass(add, NULL, NULL, NULL);
    return rez;
}

char *mx_add_with_hook(char *s, int *i, t_ost *tost) {
    char *tmp = NULL;

    *i += 1;
    while (s[*i]
           && s[*i] != '}'
           && s[*i] != '\''
           && s[*i] != '"'
           && s[*i] != '`') {
        tmp = mx_add(tmp, s[*i], i);
    }
    if (s[*i] && s[*i] == '}')
        *i += 1;
    else {
        mx_printerr("Odd number of quotes.\n");
        tost->error = 1;
        mx_free_mass(tmp, NULL, NULL, NULL);
        return NULL;
    }
    return tmp;
}

char *mx_add_with_laters(char *s, int *i) {
    char *tmp = NULL;
    
    while (s[*i] 
           && s[*i] != '}'
           && (mx_isalpha(s[*i])
           || mx_isdigit(s[*i])
           || s[*i] == '_')) {
        tmp = mx_add(tmp, s[*i], i);
    }
    return tmp;
}

int mx_check_quote(char c) {
    if (c == '"') {
        return 1;
    }
    if (c == '\'') {
        return 2;
    }
    return 0;
}
