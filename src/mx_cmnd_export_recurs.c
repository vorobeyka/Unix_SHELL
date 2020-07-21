#include "../inc/header.h"

static char *mx_set_name(char *str, int *j, int *err) {
    char *rez = NULL;
    char c[2] = { 0 };
    char *tmp = NULL;

    if (!mx_isalpha(*str) && *str != '_')
        *err = 1;
    c[1] = '\0';
    for ( ; str[*j] && str[*j] != '='; *j += 1) {
        if (!mx_isalpha(str[*j]) && !mx_isdigit(str[*j]) && str[*j] != '_')
            *err = 1;
        c[0] = str[*j];
        tmp = rez;
        rez = mx_strjoin(rez, c);
        if (tmp)
            free(tmp);
    }
    if (str[*j] == '=')
        *j += 1;
    return rez;
}

static char *mx_set_value(char *str, int *j, char *env_name) {
    char *rez = NULL;
    char c[2] = { 0 };
    char *tmp = NULL;

    c[1] = '\0';
    for ( ; str[*j]; *j += 1) {
        c[0] = str[*j];
        tmp = rez;
        rez = mx_strjoin(rez, c);
        if (tmp)
            free(tmp);
    }
    if (!rez && !getenv(env_name))
        rez = mx_strdup("''");
    else if (!rez)
        rez = mx_strdup(getenv(env_name));
    return rez;
}

int mx_cmnd_export_recurs(char **cmnd, int i, int *err) {
    if (!i)
        return 0;
    int j = 0;
    char *env_name = mx_set_name(cmnd[i], &j, err);
    char *env_value = mx_set_value(cmnd[i], &j, env_name);

    if (*err) {
        mx_print_long_err("export: ", cmnd[i], " not valid ident\n", NULL);
        mx_free_mass(env_name, env_value, NULL, NULL);
        *err = 0;
        return 1;
    }
    else
        setenv(env_name, env_value, 1);
    mx_free_mass(env_name, env_value, NULL, NULL);
    return mx_cmnd_export_recurs(cmnd, i - 1, err);
}
