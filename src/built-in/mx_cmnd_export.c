#include "header.h"

static char *set_name(char *str, int *j, int *err) {
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

static char *set_value(char *str, int *j) {
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
    return rez;
}

static int export(char **cmnd, int i, int *err) {
    if (!i) {
        return 1;
    }
    int j = 0;
    char *env_name = set_name(cmnd[i], &j, err);
    char *env_value = set_value(cmnd[i], &j);
    
    if (*err) {
        mx_print_long_err("export: ", cmnd[i], "not valid ident\n", NULL);
        *err = 0;
        return 1;
    }
    else
        setenv(env_name, env_value, 1);
    if(env_name)
        free(env_name);
    if (env_value)
        free(env_value);
    return export(cmnd, i - 1, err);
}

static char **sort_export_env() {
    extern char **environ;
    int len = mx_len_of_array(environ);
    char **rez = (char **) malloc((len + 1) * sizeof(char *));
    int flag = 1;

    for (int i = 0; environ[i]; i++)
        rez[i] = mx_strdup(environ[i]);
    rez[len] = NULL;
    while (flag) {
        flag = 0;
        for (int i = 0; rez[i + 1]; i++) {
            if (mx_strcmp(rez[i], rez[i + 1]) > 0) {
                char *tmp = rez[i];

                rez[i] = rez[i + 1];
                rez[i + 1] = tmp;
                flag = 1;
            }
        }
    }
    return rez;
}

static int print_export() {
    char **env = sort_export_env();

    for (int i = 0; env[i]; i++) {
        mx_print_long_err(env[i], "\n", NULL, NULL);
    }
    return 1;
}

// char *find_in_history(char *s, t_history *history) {
//     for (t_history *h = history; h; h->next) {
//         // if (!mx_strcmp(h->data,))
//     }
// }

// char **export_in_history(char **str, t_history *history) {
//     char **rez = NULL;

//     for ( ; *str; str++) {
//         if (mx_get_char_index(*str, '=') > 0 )
//             rez = mx_resize_one(rez, *str);
//         else
//             rez = mx_resize_one(rez, find_in_history(*str, history));
//     }
//     return str;
// }

int mx_cmnd_export(t_built *u, t_history *history) {
    int len = mx_len_of_array(u->commands) - 1;
    int error_value = 0;
    char **commands = u->commands;//export_in_history(u->commands, history);
    t_history *sdf = history;
    sdf = NULL;
    if (!len) {
        return print_export();
    }
    return export(commands, len, &error_value);
}

int mx_export(char **argv, t_ost *tost) {
    tost->built->commands = argv;
    for (t_history *h = tost->history; h; h = h->next) {
        mx_printstr(h->data);
        mx_printchar('\t');
    }
    exit(1);
    return mx_cmnd_export(tost->built, tost->history);
}
