#include "header.h"

static void dell_space(char *src, int position) {
    if (src) {
        for (int i = position; i > 0; i--) {
            printf("%c[1C", 27);
            fflush(stdout);
        }
        for (int lenn = mx_strlen(src); lenn > 0; lenn--)
            mx_long_print("\b", " ", "\b", NULL);
    }
}

static void print_data(char *src) {
    if (src)
        for (int lenn = 0; lenn < mx_strlen(src); lenn++)
            mx_printchar(src[lenn]);
}

static t_history *buffer_list(t_history **node) {
    t_history *new = NULL;
    t_history *j = *node;

    while (j->next)
        j = j->next;
    for (int i = 0; i < mx_lenn_list(*node); i++) {
        mx_push_list(&new, j->data);
        j = j->previos;
    }
    j = NULL;
    return new;
}

static int find_position(char *src) {
    int lenn = 0;

    if (src)
        lenn = mx_strlen(src);
    return lenn;
}

static void shift_list(t_history **node, int flag, int *position) {
    t_history *i = NULL;

    dell_space((*node)->data, *position);
    if (flag == 0)
        i = (*node)->next;
    else
        i = (*node)->previos;
    print_data(i->data);
    *position = 0;
    *node = i;
}

void mx_pluss_char(t_history *node, char *ch, int *position) {
    int old_pos = *position;

    dell_space(node->data, *position);
    mx_add_char(node, ch[0], *position);
    mx_printstr(node->data);
    *position = 0;

    for (int i = mx_strlen(node->data); i > mx_strlen(node->data) - old_pos; i--) {
        printf("%c[1D", 27);
        *position += 1;
    }
}

void mx_minus_char(t_history *node, int *position) {
    if (find_position(node->data) > 0 && *position != mx_strlen(node->data)) {
        int old_pos = *position;

        dell_space(node->data, *position);
        mx_dell_char(node, *position);
        if (node->data)
            mx_printstr(node->data);
        *position = 0;
        if (node->data)
            for (int i = mx_strlen(node->data); i > mx_strlen(node->data) - old_pos; i--) {
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
        dell_space(node->data, *position - 1);
        mx_dell_char(node, *position - 1);
        if (node->data)
            mx_printstr(node->data);
        *position = 0;
        if (node->data)
            for (int i = mx_strlen(node->data); i > mx_strlen(node->data) - old_pos; i--) {
                printf("%c[1D", 27);
                *position += 1;
            }
    }
}

static bool check_enter_exit(char *line) {
    if (!line)
        return true;
    char **s = mx_parse_all(line, NULL); // pars

    if (mx_lenn_mass(s) <= 2 && mx_strcmp(s[0], "exit") == 0) {
        mx_del_strarr(&s);
        return false;
    }
    mx_del_strarr(&s);
    return true;
}

char *mx_ush_read(t_history *old_node, t_ost *tost) {
    t_history *node = buffer_list(&old_node);
    char ch[3];
    char *s = NULL;
    int lenn = 0;
    int position = 0;

    while ((lenn = read(0, &ch, 3)) > 0) {
        if (lenn == 1) {
            if ((int)ch[0] == 4) { // ctrl+d
                if (!node->data) { // end
                    if (tost->out == 1) {
                        mx_printstr("\nush: you have suspended jobs.\n");
                        fflush(stdout);
                        tost->status = 146;
                        tost->out = 0;
                        mx_dell_full(node);
                        return NULL;
                    }
                    if (mx_cech_end()) {
                        mx_printstr("\n\n[Процесс завершен]\n");
                        fflush(stdout);
                    }
                    mx_dell_full(node);
                    mx_printstr("\n");
                    tcsetattr(0, TCSAFLUSH, &tost->old);
                    exit(1);
                }
                else // delete
                    mx_minus_back_char(node, &position);
            }
            else if (ch[0] == 3) { // ctrl+c
                mx_printstr("\n");
                tost->status = 130;
                mx_dell_full(node);
                return NULL;
            }
            else if (ch[0] == '\n') { // enter
                if (check_enter_exit(node->data))
                    mx_printstr("\n");
                if (node->data)
                    s = mx_strdup(node->data);
                mx_dell_full(node);
                return s;
            }
            else if (ch[0] == 0x7f) // backcpace
                mx_minus_char(node, &position);
            else if (ch[0] > 31 && ch[0] < 127)
                mx_pluss_char(node, ch, &position);
            fflush(stdout);
        }        
        else {
            if ((int)ch[0] == 27 && (int)ch[1] == 91 && (int)ch[2] == 65 && node->next)
                shift_list(&node, 0, &position);
            else if ((int)ch[0] == 27 && (int)ch[1] == 91 && (int)ch[2] == 66 && node->previos)
                shift_list(&node, 1, &position);
            else if ((int)ch[0] == 27 && (int)ch[1] == 91 && (int)ch[2] == 67 && position - 1 >= 0) {
                printf("%c[1C", 27);
                position--;
            }
            else if ((int)ch[0] == 27 && (int)ch[1] == 91 && (int)ch[2] == 68 && node && node->data && position + 1 <= mx_strlen(node->data)) {
                printf("%c[1D", 27);
                position++;
            }
            fflush(stdout);
        }
        ch[0] = 0;
        ch[1] = 0;
        ch[2] = 0;
    }
    return NULL;
}
