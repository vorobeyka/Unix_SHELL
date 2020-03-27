#include "header.h"

char *mx_clean_subsh(char *src) {
    if (mx_get_substr_index(src, "()") != -1) { // support or not
        char *s = mx_strnew(mx_strlen(src) - 2);

        for(int i = 0; i < mx_strlen(src) - 2; i++)
            s[i] = src[i];
        free(src);
        return s;
    }
    char *s = mx_strdup(src);

    free(src);
    return s;
}

static char *clean_cursh(char *src) {
    if (src[0] == '{' && src[mx_strlen(src) - 1] == '}') {
        char *s = mx_strnew(mx_strlen(src) - 2);

        for (int i = 0; i < mx_strlen(src) - 2; i++)
            s[i] = src[i + 1];
        return s;
    }
    return mx_strdup(src);
}

int mx_find_comamnd(char **arr) {
    for (int i = 1; arr[i]; i++) { // error no found
        if (mx_strcmp(arr[i], ">") == 0 || mx_strcmp(arr[i], ">>") == 0 ||
            mx_strcmp(arr[i], "<") == 0 || mx_strcmp(arr[i], "<<") == 0)
            i++;
        else
            return i;
    }
    return -1;
}

int mx_pre_support(t_ost *tost, char *line) {
    // first command should be a cup() rest of the arr a will join in one str
    char **s = mx_parse_all(line, tost);// pars
    char *name = mx_strdup(s[0]);
    char *command = clean_cursh(s[mx_find_comamnd(s)]);
    int result = 0;

    name = mx_clean_subsh(name);
    result = mx_support(tost, name, command);
    mx_del_strarr(&s);
    free(name);
    free(command);
    return result;
}

bool mx_chech_support(char *line) {
    if (mx_get_char_index(line, '|') != -1) // pipe or not
        return false;
    if (mx_get_substr_index(line, "()") != -1) // support or not
        return true;
    return false;
}

static t_support *create_support(char *name, char *line) {
    t_support *node = (t_support *)malloc(sizeof(t_support));

    node->name = mx_strdup(name);
    node->line = mx_strdup(line);
    node->next = NULL;
    return node;
}

static void mx_push_support(t_support **list, char *name, char *line) {
    t_support *node = create_support(name, line);

    if (!*list) {
        *list = node;
        return ;
    }
    t_support *i = *list;
    
    for (; i->next != NULL; i = i->next);
    i->next = node;
}

static void mx_change_support(t_support **list, char *name, char *line) {
    for (t_support *i = *list; i; i = i->next)
        if (mx_strcmp(i->name, name) == 0) {
            free(i->name);
            free(i->line);
            i->name = mx_strdup(name);
            i->line = mx_strdup(line);
            return ;
        }
}

static bool mx_search(t_support *list, char *name) {
    if (!list)
        return false;
    for (t_support *i = list; i; i = i->next)
        if (mx_strcmp(i->name, name) == 0)
            return true;
    return false;
}

int mx_support(t_ost *tost, char *name, char *command) {
    // add errors for return 1

    if (mx_search(tost->support, name))
        mx_change_support(&tost->support, name, command);
    else
        mx_push_support(&tost->support, name, command);
    return 0;
}

bool mx_check_command(t_support *list, char *line) {
    if (!list || mx_get_char_index(line, '|') != -1) // pipe or not
        return false;
    char *v = mx_strtrim(line);

    v = mx_clean_subsh(v);
    for (t_support *i = list; i; i = i->next) {
        if (mx_strcmp(i->name, v) == 0) {
            free(v);
            return true;
        }
    }
    free(v);
    return false;
}

bool mx_pre_check(char *src, t_ost *tost, int fl) {
    if (mx_get_char_index(src, '|') != -1) // pipe or not
        return false;
    char **s = mx_parse_all(src, tost);// pars

    for (int i = 0; s[i]; i++) {
        if (mx_strcmp(s[i], ">") == 0 || mx_strcmp(s[i], ">>") == 0 ||
            mx_strcmp(s[i], "<") == 0 || mx_strcmp(s[i], "<<") == 0)
            i++;
        else if ((fl == 0 && mx_chech_support(s[i])) ||
            (fl == 1 && mx_check_command(tost->support, s[i]))) {
            mx_del_strarr(&s);
            return true;
        }
        else
            break;
    }
    mx_del_strarr(&s);
    return false;
}

int mx_command(t_support *list, char *line, t_ost *tost) {
    int result = 0;
    char *v = mx_strdup(line);
    
    for (t_support *i = list; i; i = i->next)
        if (mx_strcmp(i->name, v) == 0)
            result = mx_start(tost, i->line);
    free(v);
    return result;
}
