#include "../inc/header.h"

static t_support *create_support(char *name, char *line) {
    t_support *node = (t_support *)malloc(sizeof(t_support));

    node->name = mx_strdup(name);
    node->line = mx_strdup(line);
    node->next = NULL;
    return node;
}

static void mx_push_support(t_support **list, char *name, char *line) {
    t_support *node = create_support(name, line);
    t_support *i = NULL;

    if (!*list) {
        *list = node;
        return ;
    }
    i = *list;
    for (; i->next != NULL; i = i->next);
    i->next = node;
}

static void mx_change_support(t_support **list,
                              char *name, char *line) {
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


int mx_support(t_ost *tost, char *name, char *command, char *line) {
    char *s = NULL;

    if (!mx_check_valid_supp(line)) {
        s = "ush: support commnad usage: name() {command}\n";
        write(2, s, mx_strlen(s));
        return 1;
    }
    if (mx_search(tost->support, name))
        mx_change_support(&tost->support, name, command);
    else
        mx_push_support(&tost->support, name, command);
    return 0;
}
