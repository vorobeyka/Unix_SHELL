#include "../inc/header.h"

static bool long_check(char ch[3], t_history *node, t_ost *tost) {
    if (((int)ch[0] == 4 && tost->out == 1 && !node->data)
        || (int)ch[0] == 3
        || ch[0] == '\n')
        return true;
    return false;
}

static void mx_auditor(char ch[3], t_history **node,
                       t_ost **tost, char **s) {
    if (((int)ch[0] == 4 && (*tost)->out == 1 && !(*node)->data)) {
        mx_printerr("\nush: you have suspended jobs.\n");
        fflush(stdout);
        (*tost)->status = 146;
        (*tost)->out = 0;
        mx_dell_full(*node);
    }
    else if ((int)ch[0] == 3)
        mx_sec_part(tost, node, s, 1);
    else if (ch[0] == '\n')
        mx_sec_part(tost, node, s, 2);
}

static void mx_last(char ch[3], t_history *node,
                    t_ost *tost, int *position) {
    if ((int)ch[0] == 4) {
        if (!node->data) {
            if (mx_cech_end()) {
                mx_printstr("\n\n[Process completed]");
                fflush(stdout);
            }
            mx_dell_full(node);
            mx_printstr("\n");
            tcsetattr(0, TCSAFLUSH, &tost->old);
            exit(tost->status);
        }
        else
            mx_minus_back_char(node, position);
    }
    else if (ch[0] == 0x7f)
        mx_minus_char(node, position);
    else if (ch[0] > 31 && ch[0] < 127)
        mx_pluss_char(node, ch, position);
    fflush(stdout);
}

static void a_net(char *c1, char *c3, char *c2) {
    *c1 = 0;
    *c2 = 0;
    *c3 = 0;
}

char *mx_ush_read(t_history *old_node, t_ost *tost) {
    t_history *node = mx_buffer_list(&old_node);
    char ch[3];
    char *s = NULL;
    int lenn = 0;
    int position = 0;

    while ((lenn = read(0, &ch, 3)) > 0) {
        if (lenn == 1) {
            if (long_check(ch, node, tost)) {
                mx_auditor(ch, &node, &tost, &s);
                return s;
            }
            mx_last(ch, node, tost, &position);
        }
        else
            mx_fir_part(ch, &position, &node);
        a_net(&ch[0], &ch[1], &ch[2]);
    }
    return NULL;
}

