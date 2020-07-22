#include "../inc/header.h"

void mx_change_last(t_history **node, char *line) {
    t_history *i = NULL;
    t_history *j = NULL;

    if (!line) {
        i = *node;
        if (i->next) {
            j = i->next;
            j->previos = NULL;
            *node = j;
        }
        else
            *node = NULL;
        i->next = NULL;
        free(i);
        return ;
    }
    (*node)->data = mx_strdup(line);
    free(line);
    line = NULL;
}

void mx_clean_history(t_history *node) {
    int lenn = mx_lenn_list(node);

    if (lenn > 30)
        mx_dell_list(&node);
}

int pre_builtin(char *src, t_ost *tost) {
    tost->trim = 1;
    int result = 0;
    char **s = mx_parse_all(src, tost);

    result = mx_builtin(s, tost);
    mx_del_strarr(&s);
    return result;
}

static void ckeck_last_slesh(char **command, int *flag) {
    if (mx_len_of_array(command) != 1)
        return ;
    for (int i = 0; command[i]; i++) {
        char **s = mx_trim_split(command[i], ' ');

        if (mx_strcmp(s[0], "exit") == 0)
            *flag = 1;
        mx_del_strarr(&s);
    }
}

int mx_start(t_ost *tost, char *com_line) {
    char **command = NULL;
    int result = 0;

    command = mx_parse_pipes_and_tz(com_line, ';');
    if (command == NULL)
        return result;
    ckeck_last_slesh(command, &tost->exit_slesh);
    for (int i = 0; command && command[i]; i++) {
        result = mx_if_else(command, i, tost);
        tost->status = result;
    }
    mx_del_strarr(&command);
    return result;
}
