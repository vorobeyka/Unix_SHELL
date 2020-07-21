#include "../inc/header.h"

char *cut_str_n(char *str) {
    char *rez = NULL;

    for (int i = 0; i < mx_strlen(str) - 1; ) {
        rez = mx_add(rez, str[i], &i);
    }
    free(str);
    return rez;
}

static char *prog_out_part(int fd) {
    char ch;
    char *str = NULL;

    while (read(fd, &ch, 1) != 0) {
        if (ch == '\n')
            str = mx_join_char(str, ' ');
        else
            str = mx_join_char(str, ch);
    }
    str[mx_strlen(str) - 1] = '\0';
    return str;
}
char *mx_prog_out(char *line, t_ost *tost) {
    if (!line)
        return NULL;
    char *str = NULL;
    int fd[2];
    int old = -1;

    if (pipe(fd) == -1)
        perror("ush: pipe");
    old = dup(STDOUT_FILENO);
    dup2(fd[1], STDOUT_FILENO);
    close(fd[1]);
    mx_start(tost, line);
    dup2(old, STDOUT_FILENO);
    close(old);
    str = prog_out_part(fd[0]);
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

char *mx_cut_subsh(char *str) {
    char *rez = NULL;

    if (str[0] == '(' && str[mx_strlen(str) - 1] == ')') {
        for (int i = 1; i < mx_strlen(str) - 1; )
            rez = mx_add(rez, str[i], &i);
        return rez;
    }
    return mx_strdup(str);
}

char **mx_parse_all(char *line, t_ost *tost) {
    char **rez = NULL;
    char *new_line = NULL;
    char **splited = NULL;
    char *tmp = NULL;

    // mx_long_print(line, "\t", NULL, NULL);
    if (line) {
        new_line = mx_cut_subsh(line);
        rez = mx_split_commands(new_line, tost);
        for (int i = 0; rez && rez[i]; i++) {
            if (rez[i][0] == '~' && i > 0 && mx_strcmp(rez[i - 1], "cd"))
                rez[i] = parse_home(rez[i]);
            tmp = mx_open_quotes(rez[i], tost);
            splited = mx_resize_one(splited, tmp);
            mx_free_mass(tmp, NULL, NULL, NULL);
        }
        mx_free_mass(new_line, rez, NULL, NULL);
    }
    return splited;
}
