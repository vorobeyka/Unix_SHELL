#include "header.h"

static void mx_change_last(t_history **node, char *line) {
    t_history *i = NULL;
    t_history *j = NULL;

    if (!line) {
        i = *node;
        if (i->next) {
            j = i->next;
            j->previos = NULL;
            *node = j;
        }
        else
            *node = NULL;
        i->next = NULL;
        free(i);
        return ;
    }
    (*node)->data = mx_strdup(line);
    free(line);
    line = NULL;
}

static void mx_clean_history(t_history *node) {
    int lenn = mx_lenn_list(node);

    if (lenn > 30)
        mx_dell_list(&node);
}

static bool mx_first_built_check(char *src, t_ost *tost) {
    if (mx_get_char_index(src, '|') != -1) // pipe or not
        return false;
    char **s = mx_parse_all(src, tost);// pars

    tost->last = 1;
    for (int i = 0; s[i]; i++) {
        if (mx_strcmp(s[i], ">") == 0 || mx_strcmp(s[i], ">>") == 0 ||
            mx_strcmp(s[i], "<") == 0 || mx_strcmp(s[i], "<<") == 0)
            i++;
        else if (mx_check_builtin(s[i])) {
            mx_del_strarr(&s);
            return true;
        }
        else
            break;
    }
    mx_del_strarr(&s);
    return false;
}

static int pre_builtin(char *src, t_ost *tost) {
    int result = 0;
    char **s = mx_parse_all(src, tost); // pars

    result = mx_builtin(s, tost);
    mx_del_strarr(&s);
    return result;
}

int mx_start(t_ost *tost, char *com_line) {
    char **command = NULL;
    int result = 0;

    command = mx_parse_pipes_and_tz(com_line, ';'); //mx_trim_split(com_line, ';'); // pars ;
    for (int i = 0; command && command[i]; i++) {
        if (mx_pre_check(command[i], tost, 0))
            result = mx_pre_support(tost, command[i]);
        else if (mx_pre_check(command[i], tost, 1))
            result = mx_command(tost->support, command[i], tost);
        else if (mx_first_built_check(command[i], tost))
            result = pre_builtin(command[i], tost);
        else
            result = mx_ush_execute(command[i], tost);
        tost->last = 0;
    }
    mx_del_strarr(&command);
    return result;
}

static void print_prompt(t_ost *tost) {
    if (tost->flag == 0)
        mx_printstr("u$h> ");
    else if (tost->flag == 1) {
        char *s = getenv("PWD"); // always pwd

        mx_printstr(s);
        mx_printstr("> ");
    }
    else if (tost->flag == 2) {
        char *s = getenv("USER"); // always user

        mx_printstr(s);
        mx_printstr("> ");
    }
    else if (tost->flag == 3) {
        mx_printstr(tost->prompt->emoji);
        mx_printstr(tost->prompt->code);
        mx_printstr(tost->prompt->phrase);
        mx_printstr("\033[0m");
        mx_printstr(tost->prompt->emoji);
        mx_printstr("> ");
    }
    else if (tost->flag == 4) {
        char unicorn[] = {0xF0, 0x9F, 0xA6, 0x84};

        mx_printstr(unicorn);
        mx_printstr("\033[1;5;31m");
        mx_printstr("ILYHA SOSI PESOS!");
        mx_printstr("\033[0m");
        mx_printstr(unicorn);
        mx_printstr("> ");
    }
}

void mx_ush_loop(t_ost *tost) {
    char *line = NULL;

    while (1) {
        print_prompt(tost);
        mx_push_list(&tost->history, NULL);
        tcsetattr(0, TCSAFLUSH, &tost->read);
        line = mx_ush_read(tost->history, tost);
        tcsetattr(0, TCSAFLUSH, &tost->old);
        mx_change_last(&tost->history, line);
        mx_clean_history(tost->history);
        if (line)
            tost->status = mx_start(tost, tost->history->data);
    }
}
