#include "../inc/header.h"

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

static void add_job_part(t_jobs *node, t_jobs **list, int *flag) {
    if (!*list) {
        *list = node;
        *flag = 1;
        return ;
    }
    if (node->num < (*list)->num) {
        node->next = *list;
        *list = node;
        *flag = 1;
        return ;
    }
}

void mx_add_job(char *name, int pid, t_jobs **list) {
    mx_chenge_flag_list(*list);
    t_jobs *node = create_jobs_list(name, pid, *list);
    t_jobs *i = NULL;
    int flag = 0;

    add_job_part(node, list, &flag);
    if (flag == 1)
        return ;
    i = *list;
    for (; i->next; i = i->next)
        if (node->num < i->next->num) {
            if (i->next)
                node->next = i->next;
            i->next = node;
            return ;
        }
    i->next = node;
}
