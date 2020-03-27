#include "header.h"

static void chenge_flag_list(t_jobs *list) {
    if (!list)
        return ;
    for (t_jobs *i = list; i; i = i->next) {
        if (i->flag == '+')
            i->flag = '-';
        else if (i->flag == '-')
            i->flag = ' ';
    }
}

static int lenn_list(t_jobs *list) {
    if (!list)
        return 0;
    int count = 0;
    
    for (t_jobs *i = list; i; i = i->next) {
        if (count + 1 != i->num) {
            return count;
            break;
        }
        count++;
    }
    return count;
}

static int max_step(t_jobs *list) {
    if (!list)
        return 0;
    int step = 0;

    for (t_jobs *i = list; i; i = i->next)
        if (i->step > step)
            step = i->step;
    return step;
}

static t_jobs *create_jobs_list(char *name, int pid, t_jobs *list) {
    t_jobs *node = (t_jobs *)malloc(sizeof(t_jobs));

    node->name = mx_strdup(name);
    node->pid = pid;
    node->num = lenn_list(list) + 1;
    node->step = max_step(list) + 1;
    node->flag = '+';
    node->next = NULL;
    return node;
}

void mx_add_job(char *name, int pid, t_jobs **list) {
    chenge_flag_list(*list);
    t_jobs *node = create_jobs_list(name, pid, *list);

    if (!*list) {
        *list = node;
        return ;
    }
    if (node->num < (*list)->num) {
        node->next = *list;
        *list = node;
        return ;
    }
    t_jobs *i = *list;
  
    for (; i->next; i = i->next) {
        if (node->num < i->next->num) {
            if (i->next)
                node->next = i->next;
            i->next = node;
            return ;
        }
    }
    i->next = node;
}

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
    for (i = list; i; i = i->next) {
        if (i->step > pre && i->step < step)
            pre = i->step;
    }
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

void mx_dell_job(t_jobs **list, int pid) {
    t_jobs *i = *list;
    t_jobs *j = NULL;

    back_change_flag(*list, position_dell(list, pid));
    for (int k = 0; k < position_dell(list, pid); k++) {
        if (k + 1 == position_dell(list, pid))
            j = i;
        i = i->next;
    }
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
