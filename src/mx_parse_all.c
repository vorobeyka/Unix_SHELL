#include "../inc/header.h"

char *cut_str_n(char *str) {
    char *rez = NULL;

    for (int i = 0; i < mx_strlen(str) - 1; ) {
        rez = mx_add(rez, str[i], &i);
    }
    free(str);
    return rez;
}

static char *prog_out_part(t_ost *tost, int fd) {
    char ch;
    char *str = NULL;

    while (read(fd, &ch, 1) > 0) {
        if (tost->pars == 1) {
            if (ch == '\n')
                str = mx_join_char(str, ' ');
            else
                str = mx_join_char(str, ch);
        }
        else {
            if (ch == '\n')
                break;
            str = mx_join_char(str, ch);
        }
    }
    if (mx_strlen(str) > 0 && str[mx_strlen(str) - 1] == '\n')
        str = cut_str_n(str);
    return str;
}

char *mx_prog_out(char **argv, t_ost *tost) {
    char *str = NULL;
    int fd[2];

    if (pipe(fd) == -1)
        perror("ush: pipe");
    pid_t pid = fork();

    if (pid == 0) {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        mx_exe(argv, tost, NULL);
    }
    else if (pid > 0)
        wait(NULL);
    close(fd[1]);
    str = prog_out_part(tost, fd[0]);
    close(fd[0]);
    return str;
}

char *parse_home(char *s) {
    char *rez = NULL;

    if (s[1] == '/' || !s[1] || s[1] == '+' || s[1] == '-') {
        rez = mx_strjoin(getenv("HOME"), &s[1]);
    }
    else {
        rez = mx_strjoin("/Users/", &s[1]);
    }
    free(s);
    return rez;
}

char **mx_parse_all(char *line, t_ost *tost) {
    char **rez = NULL;

    if (line) {
        rez = mx_split_commands(line, tost);
        for (int i = 0; rez && rez[i]; i++) {
            if (rez[i][0] == '~' && i > 0 && mx_strcmp(rez[i - 1], "cd"))
                rez[i] = parse_home(rez[i]);
            rez[i]  = mx_open_quotes(rez[i], tost);
        }
    }
    return rez;
}
