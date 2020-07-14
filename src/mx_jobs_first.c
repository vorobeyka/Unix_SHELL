#include "../inc/header.h"

static int position_dell(t_jobs **list, int pid) {
    int count = 0;

    for (t_jobs *i = *list; i->pid != pid; i = i->next)
        count++;
    return count;
}

static void find_pre_step(t_jobs *list, char c) {
    int step = -1;
    int pre = -1;
    t_jobs *i = NULL;

    for (i = list; i; i = i->next)
        if (i->flag == c)
            step = i->step;
    for (i = list; i; i = i->next)
        if (i->step > pre && i->step < step)
            pre = i->step;
    for (i = list; i; i = i->next)
        if (i->step == pre)
            i->flag = c;
}

static void back_change_flag(t_jobs *list, int num) {
    t_jobs *i = list;

    for (int j = 0; j < num; j++, i = i->next);
    if (i->flag == '-')
        find_pre_step(list, '-');
    else if (i->flag == '+') {
        find_pre_step(list, '-');
        find_pre_step(list, '+');
    }
}

static void dell_part(t_jobs *i, t_jobs *j, t_jobs **list) {
    if (!j) {
        *list = NULL;
        if (i->next)
            *list = i->next;
        free(i->name);
        free(i);
    }
    else {
        if (!i->next)
            j->next = NULL;
        else
            j->next = i->next;
        free(i->name);
        free(i);
    }
}

void mx_dell_job(t_jobs **list, int pid) {
    t_jobs *i = *list;
    t_jobs *j = NULL;

    back_change_flag(*list, position_dell(list, pid));
    for (int k = 0; k < position_dell(list, pid); k++) {
        if (k + 1 == position_dell(list, pid))
            j = i;
        i = i->next;
    }
    dell_part(i, j, list);
}
