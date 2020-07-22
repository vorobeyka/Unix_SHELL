#include "../inc/header.h"

char *cut_str_n(char *str) {
    char *rez = NULL;

    for (int i = 0; i < mx_strlen(str) - 1; ) {
        rez = mx_add(rez, str[i], &i);
    }
    free(str);
    return rez;
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
    char *s = str;

    for ( ; s && *s == ' '; s++);
    if (*s == '(' || *s == '{') {
        char c = *s == '(' ? ')' : '}';
        int i = mx_strlen(s) - 2;

        s++;
        for ( ; s[i] == ' '; i--);
        if (s[i] == c) {
            i--;
            return mx_strndup(s, i + 1);
        }
    }
    return mx_strdup(str);
}

char **mx_parse_all(char *line, t_ost *tost) {
    char **rez = NULL;
    char *new_line = NULL;
    char **splited = NULL;
    char *tmp = NULL;

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
