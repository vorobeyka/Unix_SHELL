#include "../inc/header.h"

static void find_first_part(t_ost *tost, int *result,
                            int *num, int flag) {
    if (flag == 1) {
        *num = mx_find_step_job(1, tost, 0, NULL);
        if (*num == -1) {
            mx_print_fg(NULL, 3, NULL);
            *result = 1;
        }
    }
    else if (flag == 2) {
        *num = mx_find_step_job(0, tost, 0, NULL);
        if (*num == -1) {
            mx_print_fg(NULL, 4, NULL);
            *result = 1;
        }
    }
}

static void find_sec_part(char *i, int *result,
                          int *num, t_ost *tost) {
    char *s = NULL;
        
    s = mx_find_num_name(i, 0);
    *num = mx_find_step_job(3, tost, 0, s);
    free(s);
    if (*num == -1) {
        mx_print_fg(NULL, 1, i);
        *result = 1;
    }
}

static void find_third_part(char *i, int *result,
                          int *num, t_ost *tost) {
    char *s = NULL;

    s = mx_find_num_name(i, 1);
    *num = mx_find_step_job(2, tost, atoi(s), NULL);
    free(s);
    if (*num == -1) {
        mx_print_fg(NULL, 2, i);
        *result = 1;
    }
}

static int find_job(char *src, t_ost *tost, int *result) {
    char *i = src;
    int num = -1;

    for (; *i == '%'; i += 1);
    if (i[0] == '+' || i[0] == '\0')
        find_first_part(tost, result, &num, 1);
    else if (i[0] == '-')
        find_first_part(tost, result, &num, 2);
    else if (isalpha(i[0]))
        find_sec_part(i, result, &num, tost);
    else if (isdigit(i[0]))
        find_third_part(i, result, &num, tost);
    else {
        mx_print_fg(NULL, 2, i);
        *result = 1;
    }
    return num;
}

void mx_fg_start(char *src, t_ost *tost, int *result) {
    if (mx_check_first_error(src, result)) {
        int step = find_job(src, tost, result);
        t_jobs *i = NULL;

        if (step != -1) {
            for (i = tost->jobs; i->step != step; i = i->next);
            t_jobs *node = mx_new_node(i);
            
            mx_print_fg(i, 0, NULL);
            mx_continue(tost, node);
            mx_free_new_node(&node);
        }
    }
}


