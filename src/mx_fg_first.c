#include "../inc/header.h"

void mx_print_fg(t_jobs *node, int fl, char *src) {
    if (fl == 0) {
        mx_printstr("[");
        mx_printint(node->num);
        mx_printstr("]");
        mx_printstr("    ");
        mx_long_print(" ", "continued", "  ", node->name);
        mx_printchar('\n');
    }
    else if (fl == 1)
        mx_long_error_print("fg: ", src, ": no such job\n", NULL);
    else if (fl == 2)
        mx_long_error_print("fg: job not found: ", src, "\n", NULL);
    else if (fl == 3)
        mx_long_error_print("fg: no current job\n", NULL, NULL, NULL);
    else if (fl == 4)
        mx_long_error_print("fg: no previous job\n", NULL, NULL, NULL);
}

static void mx_continue_part(t_ost **tost, t_jobs *node, int *status) {
    if (WIFSTOPPED(*status)) {
        *status = 146;
        (*tost)->out = 1;
        mx_long_print("\nush: suspended  ", node->name, "\n", NULL);
    }
    else if (WIFSIGNALED(*status)) {
        *status = 130;
        (*tost)->out = 0;
        mx_printstr("\n");
        mx_dell_job(&(*tost)->jobs, node->pid);
    }
    else if (WIFEXITED(*status)) {
        *status = WEXITSTATUS(*status);
        (*tost)->out = 0;
        mx_dell_job(&(*tost)->jobs, node->pid);
    }
}

void mx_continue(t_ost *tost, t_jobs *node) {
    pid_t shell_pid = getpid();
    int status = 0;

    if (isatty(0)) {
        tcsetpgrp(0, node->pid);
        tcsetpgrp(1, node->pid);
    }
    kill(-(node->pid), SIGCONT);
    waitpid(-(node->pid), &status, WUNTRACED);
    mx_continue_part(&tost, node, &status);
    if (isatty(0)) {
        tcsetpgrp(0, shell_pid);
        tcsetpgrp(1, shell_pid);
    }
    if (WIFSTOPPED(status)) {
        status = 146;
        tost->out = 1;
        mx_printstr("\n");
    }
}

void mx_free_new_node(t_jobs **i) {
    t_jobs *node = *i;

    free(node->name);
    free(node);
}

int mx_fg(char **argv, t_ost *tost) {
    int result = 0;

    if (mx_lenn_mass(argv) == 1)
        mx_fg_start("%+", tost, &result);
    else
        for (int i = 1; argv[i]; i++)
            mx_fg_start(argv[i], tost, &result);
    return result;
}
