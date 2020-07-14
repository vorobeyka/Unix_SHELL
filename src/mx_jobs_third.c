#include "../inc/header.h"

void mx_chenge_flag_list(t_jobs *list) {
    if (!list)
        return ;
    for (t_jobs *i = list; i; i = i->next) {
        if (i->flag == '+')
            i->flag = '-';
        else if (i->flag == '-')
            i->flag = ' ';
    }
}
