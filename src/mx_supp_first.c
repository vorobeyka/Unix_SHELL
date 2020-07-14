#include "../inc/header.h"

bool mx_chech_support(char **arr) {
    if (mx_len_of_array(arr) == 2
        && arr[0][mx_strlen(arr[0]) - 1] == ')'
        && arr[0][mx_strlen(arr[0]) - 2] == '('
        && arr[1][0] == '{'
        && arr[1][mx_strlen(arr[1]) - 1] == '}')
        return true;
    return false;
}

bool mx_check_command(t_support *list, char **arr) {
    if (mx_len_of_array(arr) == 1)
        for (t_support *i = list; i; i = i->next)
            if (mx_strcmp(i->name, arr[0]) == 0)
                return true;
    return false;
}

bool mx_pre_check(char *line, t_ost *tost, int fl) {
    char **pipe = mx_parse_pipes_and_tz(line, '|');

    if (mx_lenn_mass(pipe) > 1) {
        mx_del_strarr(&pipe);
        return false;
    }
    for (int j = 0; pipe[j]; j++) {
        char **arr = mx_parse_pipes_and_tz(pipe[j], ' ');

        if ((fl == 0 && mx_chech_support(arr))
            || (fl == 1 && mx_check_command(tost->support, arr))) {
            mx_del_strarr(&arr);
            mx_del_strarr(&pipe);
            return true;
        }
        mx_del_strarr(&arr);
    }
    mx_del_strarr(&pipe);
    return false;
}

int mx_command(t_support *list, char *line, t_ost *tost) {
    int result = 0;
    char *z = NULL;
    char *v = mx_strdup(line);
    
    for (t_support *i = list; i; i = i->next)
        if (mx_strcmp(i->name, v) == 0) {
            z = i->line;
            i->line = mx_strtrim(i->line);
            free(z);
            result = mx_start(tost, i->line);
        }
    free(v);
    return result;
}
