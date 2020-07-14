#include "../inc/header.h"

void mx_dell_space(char *src, int position) {
    if (src) {
        for (int i = position; i > 0; i--) {
            printf("%c[1C", 27);
            fflush(stdout);
        }
        for (int lenn = mx_strlen(src); lenn > 0; lenn--)
            mx_long_print("\b", " ", "\b", NULL);
    }
}

void mx_print_data(char *src) {
    if (src)
        for (int lenn = 0; lenn < mx_strlen(src); lenn++)
            mx_printchar(src[lenn]);
}

t_history *mx_buffer_list(t_history **node) {
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

static bool check_exit_part(char ***pipe, char ***tz, int j) {
        char **arr = mx_parse_pipes_and_tz(*pipe[j], ' ');

        if (arr && mx_lenn_mass(arr) <= 2 && mx_strcmp(arr[0], "exit") == 0) {
            mx_del_strarr(&arr);
            mx_del_strarr(pipe);
            mx_del_strarr(tz);
            return false;
        }
        mx_del_strarr(&arr);
    return true;
}

bool mx_check_enter_exit(char *line) {
    if (!line)
        return true;
    char **tz = mx_parse_pipes_and_tz(line, ';');

    for (int i = 0; tz && tz[i]; i++) {
        char **pipe = mx_parse_pipes_and_tz(tz[i], '|');

        if (mx_lenn_mass(pipe) > 1) {
            mx_del_strarr(&pipe);
            break;
        }
        for (int j = 0; pipe[j]; j++)
            if (!check_exit_part(&pipe, &tz, j))
                return false;
        mx_del_strarr(&pipe);
    }
    mx_del_strarr(&tz);
    return true;
}

