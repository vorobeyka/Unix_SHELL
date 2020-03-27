#include "header.h"

char *parse_slash(char *str, char *s, int *i) {
    char *rez = mx_strdup(str);

    rez = mx_add(rez, s[*i + 1], i);
    *i += 1;
    if (str)
        free(str);
    return rez;
}

char *mx_split_quotes(char *add, char *str, char c, int *i) {
    char *rez = add ? mx_strdup(add) : NULL;

    rez = mx_add(rez, str[*i], i);
    for ( ; str[*i] && str[*i] != c; )
        rez = mx_add(rez, str[*i], i);
    if (!str[*i]) {
        mx_printerr("Odd number of quote\n");
        exit(1);
    }
    rez = mx_add(rez, str[*i], i);
    if (add)
        free(add);
    return rez;
}

void mx_shift_spaces(char *str, int *i) {
    if (str[*i] == ' ') {
        *i += 1;
        mx_shift_spaces(str, i);
    }
}

void mx_find_delim(char *str, char delim, int *i) {
    if (str[*i] && str[*i] != delim) {
        *i += 1;
        mx_find_delim(str, delim, i);
    }
}

void mx_last_delim(char *str, char delim, int *i) {
    if (str[*i] && str[*i] == delim) {
        *i += 1;
        mx_last_delim(++str, delim, i);
    }
}

int check_delims(char c) {
    if (c == '\'' || c == '"' || c == '`') {
        return 0;
    }
    else if (c == '{') {
        return 1;
    }
    else if (c == '$') {
        return 2;
    }
    return 3;
}

char *mx_split_hooks_baks(char *add, char *str, int *i) {
    char *rez = add ? mx_strdup(add) : NULL;

    rez = mx_add(rez, str[*i], i);
    for ( ; str[*i] && str[*i] != ' '; ) {
        rez = str[*i] == '\'' || str[*i] == '"'
            ? mx_split_quotes(rez, str, str[*i], i)
            : mx_add(rez, str[*i], i);
    }
    if (add)
        free(add);
    return rez;
}

char **mx_split_commands(char *str) {
    char **rez = NULL;
    char *add = NULL;
    int token;

    for (int i = 0; str && str[i] != '\0'; ) {
        mx_shift_spaces(str, &i);
        token = check_delims(str[i]);
        add = str[i] == '\\' ? parse_slash(add, str, &i)
              : !token ? mx_split_quotes(add, str, str[i], &i)
              : token < 3 ? mx_split_hooks_baks(add, str, &i)
              : mx_add(add, str[i], &i);
        if (!str[i] || str[i] == ' ') {
            rez = mx_resize_one(rez, add);
            if (add)
                free(add);
            add = NULL;
        }
    }
    return rez;
}


//ДЛЯ ДОЛЛАРА

bool check_err(char *s) {
    for (int i = 0; s[i] && s[i] != '}'; i++) {
        if (check_delims(s[i]) != 3 || s[i] == ',') {
            return true;
        }
    }
    return false;
}

char *add_with_hook(char *s, int *i) {
    char *tmp = NULL;

    *i += 1;
    while (s[*i] && s[*i] != '}' && s[*i] != '\'' && s[*i] != '"' && s[*i] != '`')
        tmp = mx_add(tmp, s[*i], i);
    if (s[*i] && s[*i] == '}')
        *i += 1;
    return tmp;
}

char *add_with_laters(char *s, int *i) {
    char *tmp = NULL;
    
    while (s[*i] && s[*i] != '}'
        && (mx_isalpha(s[*i]) || mx_isdigit(s[*i]) || s[*i] == '_'))
        tmp = mx_add(tmp, s[*i], i);
    return tmp;
}

char *parse_env(char *s, int *i) {
    char *rez = NULL;
    char *tmp = NULL;

    *i += 1;
    if (s[*i] == '{' && check_err(&s[*i + 1])) {
        mx_printerr("u$s: bad substitution\n");
        exit(1);
    }
    if (s[*i] == '{')
        tmp = add_with_hook(s, i);
    else
        tmp = add_with_laters(s, i);
    if (tmp && getenv(tmp)) {
        rez = getenv(tmp);
        free(tmp);
    }
    return rez;
}

bool check_first_baks(char *s) {
    if (!s) {
        return false;
    }
    if (mx_get_char_index(s, '\'') < 0) {
        return true;
    }
    if (mx_get_char_index(s, '$') != -1
        && mx_get_char_index(s, '$') < mx_get_char_index(s, '\'')) {
        return true;
    }
    return false;
}

char *parse_baks(char *s, int *i) {
    if (s[*i + 1] && (check_delims(s[*i + 1]) == 3 || check_delims(s[*i + 1]) == 1)) {
        return parse_env(s, i);
    }
    return NULL;
}
//--------------------------------------------------$$$$$$$$$$$$$$$$$$$$-------------

int check_quote(char c) {
    if (c == '"') {
        return 1;
    }
    if (c == '\'') {
        return 2;
    }
    return 0;
}

char *parse_odd(char *s, int *i) {
    char *rez = NULL;

    while (s[*i] && s[*i] != '\'')
        rez = mx_add(rez, s[*i], i);
    *i += 1;
    return rez;
}

char *parse_not_odd(char *s, int *i, t_ost *tost) {
    char *rez = NULL;

    while (s[*i] && s[*i] != '"') {
        if (s[*i] == '$') {
            if (s[*i + 1] == '?') {
                rez = mx_super_join(rez, mx_itoa(0), 3); //ВПИСАТЬ КОЕ ЧЕ
                *i += 2;
            }
            else
                rez = mx_super_join(rez, parse_baks(s, i), 3);
        }
        // СЮДА НУЖНО ВПИХНУТЬ НА ОБРАБОТКУ КОМАНДЫ
        else
            rez = mx_add(rez, s[*i], i);
    }
    *i += 1;
    return rez;
}

char *add_quotes(char *str, char *s, int *i, t_ost *tost) {
    char *rez = NULL;
    char *tmp = NULL;

    *i += 1;
    if (s[*i - 1] == '\'') {
        rez = mx_super_join(str, parse_odd(s, i), 3);
    }
    else {
        rez = mx_super_join(str, parse_not_odd(s, i, tost), 3);
    }
    return rez;
}


char *mx_prog_out(char **argv, t_ost *tost) {
    char *str = NULL;
    char ch;
    int fd[2];

    if (pipe(fd) == -1)
        perror("ush: pipe");
    pid_t pid = fork();

    if (pid < 0)
        perror("ush: fork");
    else if (pid == 0) {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        mx_exe(argv, tost);
    }
    else {
        wait(NULL);
        close(fd[1]);
        while (read(fd[0], &ch, 1) > 0)
            str = mx_join_char(str, ch);
        close(fd[0]);
    }
    return str;
}

char *parse_command(char *str, char *s, int *i, t_ost *tost) {
    char *rez = mx_strdup(str);
    char *command = NULL;

    *i += 1;
    if (str)
        free(str);
    while (s && s[*i] && s[*i] != '`')
        command = mx_add(command, s[*i], i);
    if (s[*i] == '`')
        *i += 1;
    return mx_super_join(rez, mx_prog_out(mx_parse_all(command, tost), tost), 3);
}



char *open_quotes(char *s, t_ost *tost) {
    char *rez = NULL;

    for (int i = 0 ; s[i]; ) {
        if (s[i] == '$') {
            if (s[i + 1] == '?') {
                rez = mx_super_join(rez, mx_itoa(0), 3); // ВПИХНУТЬ ПОСЛЕДНИЙ РЕЗ
                i += 2;
            }
            else
                rez = mx_super_join(rez, parse_baks(s, &i), 3);
        }
        else if (s[i] == '`') {
            rez = parse_command(rez, s, &i, tost);
        }
        else {
            rez = check_quote(s[i]) > 0 ?
                  add_quotes(rez, s, &i, tost) :
                  mx_add(rez, s[i], &i);
        }
    }
    free(s);
    return rez;
}

char *parse_home(char *s) {
    char *rez = NULL;

    if (s[1] == '/' || !s[1] || s[1] == '+' || s[1] == '-') {
        rez = mx_strjoin(getenv("HOME"), &s[1]);
    }
    else {
        rez = mx_strjoin("/home/", &s[1]);
    }
    free(s);
    return rez;
}

char **mx_parse_all(char *line, t_ost *tost) {
    char **rez = NULL;

    if (line) {
        rez = mx_split_commands(line);
        for (int i = 0; rez && rez[i]; i++) {
            if (rez[i][0] == '~') {
                if (mx_strcmp(rez[i - 1], "cd"))
                    rez[i] = parse_home(rez[i]);
            }
            rez[i]  = open_quotes(rez[i], tost);
        }
    }
    return rez;
}
