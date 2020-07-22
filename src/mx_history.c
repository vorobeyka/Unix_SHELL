#include "../inc/header.h"

static t_history *create_node(char *data) {
    t_history *node = (t_history *)malloc(sizeof(t_history));

    if (!data)
        node->data = NULL;
    else
        node->data = mx_strdup(data);
    node->next = NULL;
    node->previos = NULL;
    return node;
}

void mx_push_list(t_history **list, char *data) {
    t_history *node = create_node(data);
    t_history *i = *list;

    if (i == NULL) {
        *list = node;
        return ;
    }
    i->previos = node;
    node->next = *list;
    *list = node;
}

int mx_lenn_list(t_history *list) {
    int lenn = 0;

    for (t_history *i = list; i; i = i->next)
        lenn++;
    return lenn;
}

void mx_dell_list(t_history **list) {
    t_history *i = NULL;
    t_history *j = NULL;

    if (*list == NULL)
        return ;
    i = *list;
    while (i->next != NULL)
        i = i->next;
    if (i->previos) {
        j = i->previos;
        j->next = NULL;
    }
    i->previos = NULL;
    free(i->data);
    free(i);
}

void mx_dell_full(t_history *list) {
    if (!list)
        return ;
    while (list->previos != NULL)
        list = list->previos;
    int lenn = mx_lenn_list(list);

    for (int i = 0; i < lenn; i++)
        mx_dell_list(&list);
}
