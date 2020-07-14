#include "../inc/header.h"

void mx_add_char(t_history *node, char ch, int position) {
    int lenn = 0;
    char *s = NULL;

    if (node && node->data)
        lenn = mx_strlen(node->data);
    s = mx_strnew(lenn + 1);
    s = mx_strncpy(s, node->data, lenn);
    for (int i = lenn + 1; i > lenn - position; i--)
        s[i] = s[i - 1];
    s[lenn - position] = ch;
    free(node->data);
    node->data = NULL;
    node->data = mx_strdup(s);
    free(s);
    s = NULL;
}

static void dell_char_part(t_history **node, char **s) {
    free((*node)->data);
    (*node)->data = NULL;
    (*node)->data = mx_strdup(*s);
    free(*s);
    *s = NULL;
}

void mx_dell_char(t_history *node, int position) {
    int lenn = 0;
    char *s = NULL;

    if (node && node->data)
        lenn = mx_strlen(node->data);
    if (lenn - 1 > 0) {
        s = mx_strnew(lenn - 1);
        for (int i = 0; i < lenn - position - 1; i++)
            s[i] = node->data[i];
        for (int i = lenn; i > lenn - position - 1; i--)
            s[i - 1] = node->data[i];
        dell_char_part(&node, &s);
    }
    else {
        free(node->data);
        node->data = NULL;
    }
}
