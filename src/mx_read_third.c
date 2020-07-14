#include "../inc/header.h"

static int find_position(char *src) {
    int lenn = 0;

    if (src)
        lenn = mx_strlen(src);
    return lenn;
}

void mx_shift_list(t_history **node, int flag, int *position) {
    t_history *i = NULL;

    mx_dell_space((*node)->data, *position);
    if (flag == 0)
        i = (*node)->next;
    else
        i = (*node)->previos;
    mx_print_data(i->data);
    *position = 0;
    *node = i;
}

void mx_pluss_char(t_history *node, char *ch, int *position) {
    int old_pos = *position;

    mx_dell_space(node->data, *position);
    mx_add_char(node, ch[0], *position);
    mx_printstr(node->data);
    *position = 0;
    for (int i = mx_strlen(node->data);
         i > mx_strlen(node->data) - old_pos; i--) {
        printf("%c[1D", 27);
        *position += 1;
    }
}

void mx_minus_char(t_history *node, int *position) {
    if (find_position(node->data) > 0
        && *position != mx_strlen(node->data)) {
        int old_pos = *position;

        mx_dell_space(node->data, *position);
        mx_dell_char(node, *position);
        if (node->data)
            mx_printstr(node->data);
        *position = 0;
        if (node->data)
            for (int i = mx_strlen(node->data);
                 i > mx_strlen(node->data) - old_pos; i--) {
                printf("%c[1D", 27);
                *position += 1;
            }
    }
}

void mx_minus_back_char(t_history *node, int *position) {
    if (*position != 0) {
        int old_pos = *position - 1;

        for (int i = *position; i > 0; i--)
            mx_printstr(" ");
        for (int i = *position - 1; i > 0; i--)
            mx_long_print("\b", " ", "\b", NULL);
        mx_dell_space(node->data, *position - 1);
        mx_dell_char(node, *position - 1);
        if (node->data)
            mx_printstr(node->data);
        *position = 0;
        if (node->data)
            for (int i = mx_strlen(node->data);
                i > mx_strlen(node->data) - old_pos; i--) {
                printf("%c[1D", 27);
                *position += 1;
            }
    }
}

