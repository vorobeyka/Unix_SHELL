#include "../inc/header.h"

t_jobs *mx_new_node(t_jobs *i) {
    t_jobs *node = (t_jobs *)malloc(sizeof(t_jobs));

    node->flag = i->flag;
    node->name = mx_strdup(i->name);
    node->next = NULL;
    node->num = i->num;
    node->pid = i->pid;
    node->step = i->step;
    return node;
}

bool mx_check_first_error(char *src, int *result) {
    if (src[0] == '%' || iswalpha(src[0]))
        return true;
    *result = 1;
    mx_print_fg(NULL, 2, src);
    return false;
}

char *mx_find_num_name(char *src1, int fl) {
    char *s = NULL;

    for (int i = 0; src1[i]; i++) {
        if ((isalpha(src1[0]) && fl == 0)
        || (isdigit(src1[0]) && fl == 1))
            s = mx_join_char(s, src1[i]);
        else
            break;
    }
    return s;
}

int mx_find_step_job(int fl, t_ost *tost, int pos, char *src) {
    if (!tost->jobs)
        return -1;
    int num = -1;
    t_jobs *i = NULL;

    for (i = tost->jobs; i; i = i->next) {
        if ((fl == 0 && i->flag == '-')
            || (fl == 1 && i->flag == '+'))
            num = i->step;
        else if (fl == 2 && i->num == pos)
            num = i->step;
        else if (fl == 3 && strncmp(i->name, src, mx_strlen(src)) == 0)
            num = i->step;
    }
    return num;
}
