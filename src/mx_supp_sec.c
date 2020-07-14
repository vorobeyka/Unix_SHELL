#include "../inc/header.h"

char *mx_command_find(char *str) {
    char *s = NULL;

    for (int i = mx_get_char_index(str, '{') + 1;
        i < mx_get_char_index(str, '}'); i++)
        s = mx_join_char(s, str[i]);
    return s;
}

char *mx_clean_subsh(char *src) {
    char *new = mx_strnew(mx_strlen(src) - 2);

    for (int i = 0; i < mx_strlen(src) - 2; i++)
        new[i] = src[i + 1];
    return new;
}

int mx_pre_support(t_ost *tost, char *line) {
    char **s = mx_parse_pipes_and_tz(line, ' ');
    char *name = mx_strnew(mx_strlen(s[0]));
    char *command = mx_clean_subsh(s[1]);
    int result = 0;

    for (int i = 0; s[0][i] && s[0][i] != '('; i++)
        name[i] = s[0][i];
    result = mx_support(tost, name, command, line);
    mx_del_strarr(&s);
    free(name);
    free(command);
    return result;
}

bool mx_check_valid_supp(char *line) {
    if (mx_get_substr_index(line, "() ") < 0
        || mx_get_substr_index(line, " {") < 0
        || line[mx_strlen(line) - 1] != '}')
        return false;
    return true;
}

