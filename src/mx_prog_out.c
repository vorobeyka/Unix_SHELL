#include "../inc/header.h"

static char *prog_out_read_return(char *rez) {
    char *tmp = NULL;
    int i = 0;

    if (!rez) {
        return NULL;
    }
    mx_shift_spaces(rez, &i);
    if (rez[i] == '\0') {
        mx_free_mass(rez, NULL, NULL, NULL);
        return NULL;
    }
    tmp = mx_strtrim(&rez[i]);
    mx_free_mass(rez, NULL, NULL, NULL);
    return tmp;
}

static char *prog_out_read(int fd) {
    char ch;
    char *rez = NULL;

    while (read(fd, &ch, 1) != 0) {
        if (ch == '\n')
            rez = mx_join_char(rez, ' ');
        else
            rez = mx_join_char(rez, ch);
    }
    return prog_out_read_return(rez);
}

static char *prog_out_part(int fd, t_ost *tost) {
    char *str = prog_out_read(fd);

    if (!str) {
        return NULL;
    }
    if (tost->trim == 0) {
        char **arr = mx_parse_pipes_and_tz(str, ' ');

        mx_strdel(&str);
        str = mx_strdup(arr[0]);
        mx_del_strarr(&arr);
    }
    return str;
}

char *mx_prog_out(char *line, t_ost *tost) {
    if (!line) {
        return NULL;
    }
    char *str = NULL;
    int fd[2];
    int old = -1;

    tost->kostil_x2 = true;
    if (pipe(fd) == -1)
        perror("ush: pipe");
    old = dup(STDOUT_FILENO);
    dup2(fd[1], STDOUT_FILENO);
    close(fd[1]);
    mx_start(tost, line);
    dup2(old, STDOUT_FILENO);
    close(old);
    str = prog_out_part(fd[0], tost);
    close(fd[0]);
    return str;
}
