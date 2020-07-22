#include "../inc/header.h"

bool mx_first_built_check(char *src) {
    char **pipe = mx_parse_pipes_and_tz(src, '|');

    if (mx_lenn_mass(pipe) > 1) {
        mx_del_strarr(&pipe);
        return false;
    }
    for (int j = 0; pipe && pipe[j]; j++) {
        char **arr = mx_parse_pipes_and_tz(pipe[j], ' ');

        if (arr && mx_check_builtin(arr[0])) {
            mx_del_strarr(&arr);
            mx_del_strarr(&pipe);
            return true;
        }
        mx_del_strarr(&arr);
    }
    mx_del_strarr(&pipe);
    return false;
}

static void what_i_need_to_print(int flag, t_ost *tost) {
    char *s = NULL;

    if (flag == 1)
        s = getenv("PWD");
    else if (flag == 2)
        s = getenv("USER");
    else if (flag == 3) {
        mx_printstr(tost->prompt->emoji);
        mx_printstr(tost->prompt->code);
        mx_printstr(tost->prompt->phrase);
        mx_printstr("\033[0m");
        mx_printstr(tost->prompt->emoji);
        mx_printstr("> ");
    }
    mx_printstr(s);
    mx_printstr("> ");
}

void mx_print_echo_kostil(int *value) {
    switch (*value) {
    case 1:
        mx_printstr("%\n");
        break;
    default:
        mx_printstr("\n");
        break;
    }
    *value = 0;
}

void mx_print_prompt(t_ost *tost) {
    char unicorn[] = {0xF0, 0x9F, 0xA6, 0x84, '\0'};

    tost->kostil_x2 = false;
    if (tost->kostil) {
        mx_printstr("\n");
        tost->kostil = false;
    }
    if (tost->flag == 0)
        mx_printstr("u$h> ");
    else if (tost->flag >= 1 && tost->flag <= 3)
        what_i_need_to_print(tost->flag, tost);
    else if (tost->flag == 4) {
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
        mx_print_prompt(tost);
        mx_push_list(&tost->history, NULL);
        tcsetattr(0, TCSAFLUSH, &tost->read);
        line = mx_ush_read(tost->history, tost);
        tcsetattr(0, TCSAFLUSH, &tost->old);
        mx_change_last(&tost->history, line);
        mx_clean_history(tost->history);
        if (line) {
            tost->trim = 0;
            tost->status = mx_start(tost, tost->history->data);
        }
    }
}
