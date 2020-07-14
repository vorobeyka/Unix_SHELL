#include "../inc/header.h"

static bool fir_part_fir(char ch[3], t_history *node, int flag) {
    if (flag == 1
        && (int)ch[0] == 27
        && (int)ch[1] == 91
        && (int)ch[2] == 65
        && node->next)
        return true;
    else if (flag == 2
             && (int)ch[0] == 27
             && (int)ch[1] == 91
             && (int)ch[2] == 66
             && node->previos)
        return true;
    return false;
}

static bool fir_part_sec(char ch[3], int position,
                         t_history *node, int flag) {
    if (flag == 1
        && (int)ch[0] == 27
        && (int)ch[1] == 91
        && (int)ch[2] == 67
        && position - 1 >= 0)
        return true;
    else if (flag == 2
             && (int)ch[0] == 27
             && (int)ch[1] == 91
             && (int)ch[2] == 68
             && node
             && node->data
             && position + 1 <= mx_strlen(node->data))
        return true;
    return false;
}

void mx_fir_part(char ch[3], int *position, t_history **node) {
    if (fir_part_fir(ch, *node, 1))
        mx_shift_list(node, 0, position);
    else if (fir_part_fir(ch, *node, 2))
        mx_shift_list(node, 1, position);
    else if (fir_part_sec(ch, *position, *node, 1)) {
        printf("%c[1C", 27);
        *position -= 1;
    }
    else if (fir_part_sec(ch, *position, *node, 2)) {
        printf("%c[1D", 27);
        *position += 1;
    }
    fflush(stdout);
}

void mx_sec_part(t_ost **tost, t_history **node, char **s, int flag) {
    if (flag == 1) {
        mx_printstr("\n");
        (*tost)->status = 130;
        mx_dell_full(*node);
    }
    else if (flag == 2) {
        if (mx_check_enter_exit((*node)->data))
            mx_printstr("\n");
        if ((*node)->data)
            *s = mx_strdup((*node)->data);
        mx_dell_full(*node);
    }
}

