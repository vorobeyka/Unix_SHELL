#include "../inc/header.h"

static bool chech_pid(t_jobs *list, int pid) {
    for (t_jobs *i = list; i; i = i->next)
        if (i->pid == pid)
            return true;
    return false;
}

static void kill_part(int num, t_ost *tost, int *result) {
    if (chech_pid(tost->jobs, num)) {
        if (kill(num, SIGKILL) == -1) {
            perror("kill: ");
            *result = 1;
        }
        else {
            wait(NULL);
            mx_dell_job(&tost->jobs, num);
            if (!tost->jobs)
                tost->out = 0;
        }
    }
    else {
        mx_printerr("kill: illegal pid: ");
        mx_printint(num);
        mx_printerr("\n");
        *result = 1;
    }
}

int mx_kill(char **argv, t_ost *tost) {
    int result = 0;

    for (int i = 1; argv[i]; i++) {
        int num = mx_atoi(argv[i]);

        kill_part(num, tost, &result);
    }
    return result;
}
