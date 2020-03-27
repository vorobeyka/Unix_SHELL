#include "header.h"

int mx_builtin(char **argv, t_ost *tost) {
    char *builtin[] = {"env", "cd", "pwd", "which", "echo", "export", "unset", "fg", "exit", "true", "false", "jobs", "kill", "change", "next", NULL};
    int (*builtin_func[]) (char **, t_ost *tost) = {&mx_env, &mx_cd, &mx_pwd, &mx_which, &mx_echo, &mx_export, &mx_unset, &mx_fg, &mx_exit, &mx_true, &mx_false, &mx_jobs, &mx_kill, &mx_change, &mx_next};
    int result = 0;

    for (int i = 0; builtin[i]; i++)
        if (mx_strcmp(argv[0], builtin[i]) == 0)
            result = (*builtin_func[i]) (argv, tost);
    return result;
}

bool mx_check_builtin(char *name) {
    char *builtin[] = {"env", "cd", "pwd", "which", "echo", "export", "unset", "fg", "exit", "true", "false", "jobs", "kill", "change", "next", NULL};
    
    for (int i = 0; builtin[i]; i++)
        if (mx_strcmp(builtin[i], name) == 0)
            return true;
    return false;
}

// int mx_export(char **argv, t_ost *tost) {
//     if (tost->env)
//         printf("%s\n", argv[0]);
//     return 0;
// }

// int mx_unset(char **argv, t_ost *tost) {
//     if (tost->env)
//         printf("%s\n", argv[0]);
//     return 0;
// }

int mx_exit(char **argv, t_ost *tost) {
    if (mx_lenn_mass(argv) > 2) {
        mx_printerr("exit: too many arguments\n");
        return 1;
    }
    if (tost->out == 1) {
        mx_printstr("\nush: you have suspended jobs.\n");
        tost->out = 0;
        return 0;
    }
    int result = 0;

    if (argv[1]) {
        result = atoi(argv[1]);
        for (; result > 255; result -= 255)
            result -= 1;
        for (; result < 0; result += 255)
            result += 1;
    }
    else
        result = tost->status;
    if (mx_cech_end())
        mx_printstr("\n\n[Процесс завершен]\n");
    if (tost->last == 1)
        mx_printstr("\n");
    exit(result);
    return 0;
}

// int mx_cd(char **argv, t_ost *tost) {
//     if (tost->env)
//         printf("%s\n", argv[0]);
//     return 0;
// }

// int mx_pwd(char **argv, t_ost *tost) {
//     if (tost->env)
//         printf("%s\n", argv[0]);
//     return 0;
// }

int mx_which(char **argv, t_ost *tost) {
    if (tost->env)
        printf("%s\n", argv[0]);
    return 0;
}

// int mx_echo(char **argv, t_ost *tost) {
//     if (tost->env) {
//         printf("%d\n", tost->status);
//         printf("\n");
//         printf("%s\n", argv[0]);
//     }
//     return 0;
// }






















// My built-in

static int find_flag(char *s) {
    int flag = -1;

    if (!s || (s && s[0] != '-'))
        flag = 0;
    else if (s) {
        if (s[0] == '-' && s[1] == 'u')
            flag = 1;
        else if (s[0] == '-' && s[1] == 'P')
            flag = 2;
        else if (s[0] == '-' && (s[1] == '\0' || s[1] == 'i'))
            flag = 3;
    }
    return flag;
}

char **mx_option_loop(char **shift, int *result) {
    while (1) {
        shift += 1;
        int flag = find_flag(*shift);

        if (flag == -1 || flag == 0) {
            if (flag == -1)
                *result = 1;
            return shift;
        }
        else if (flag == 3)
            clearenv();
        else if (flag == 2 || flag == 1) {
            shift += 1;
            if (!*shift) {
                printf("need argument\n"); // error need argument;
                return shift;
            }
            else if (flag == 2)
                setenv("PATH", *shift, 1);
            else if (flag == 1)
                unsetenv(*shift);
        }
    }
    return shift;
}

static bool check_env(char *src) {
    char **s = mx_strsplit(src, '='); // split =

    if (mx_lenn_mass(s) != 2) {
        mx_del_strarr(&s);
        return true;
    }
    mx_del_strarr(&s);
    return false;
}

char **mx_add_env(char **shift, t_list **list) {
    if (shift && *shift)
        while (1) {
            if (!*shift || check_env(*shift)) // split =
                return shift;
            putenv(*shift);
            mx_push_back(list, *shift);
            shift += 1;
        }
    return NULL;
}

static void mx_print_env(void) {
    extern char **environ;

    for (int i = 0; environ[i]; i++) {
        mx_printstr(environ[i]);
        mx_printstr("\n");
    }
}

static void clean_list(t_list **list) {
    if (!*list)
        return ;
    int lenn = 0;
    t_list *i = *list;
    t_list *j = NULL;
  
    for (; i->next; i = i->next)
        lenn++;
    for (; lenn >= 0; lenn--) {
        i = *list;
        for (; i->next; i = i->next)
            if (!i->next->next)
                j = i;
        if (j) {
            j->next = NULL;
            j = NULL;
        }
        free(i->data);
        free(i);
        i = NULL;
    }
    *list = NULL;
}

int mx_env(char **argv, t_ost *tost) {
    if (mx_lenn_mass(argv) == 1) {
        mx_print_env();
        return 0;
    }
    pid_t pid = fork();
    int result = 0;
    int status = 0;

    if (pid < 0)
        perror("ush: fork");
    else if (pid == 0) {
        char **shift = argv;
        t_list *list = NULL;

        shift = mx_option_loop(shift, &result); // change shift {a=d}=c $(a=b)=c "a=b"=c 'a=b'=c `a=b`=c
        if (result == 1) {
            printf("invalid flag\n"); // print error
            exit(result);
        }
        shift = mx_add_env(shift, &list);
        if (*shift) {
            clean_list(&list);
            mx_exe(shift, tost);
        }
        else {
            for (t_list *i = list; i; i = i->next)
                mx_long_print(i->data, "\n", NULL, NULL);
            clean_list(&list);
        }
        exit(0);
    }
    else
        waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        result = WEXITSTATUS(status);
    return result;
}





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

void mx_continue(t_ost *tost, t_jobs *node) {
    pid_t shell_pid = getpid();
    int status = 0;

    tcsetpgrp(0, node->pid);
    tcsetpgrp(1, node->pid);
    kill(node->pid, SIGCONT);
    waitpid(node->pid, &status, WUNTRACED);
    if (WIFSTOPPED(status)) {
        status = 146;
        tost->out = 1;
        mx_long_print("\nush: suspended  ", node->name, "\n", NULL);
    }
    else if (WIFSIGNALED(status)) {
        status = 130;
        tost->out = 0;
        mx_printstr("\n");
        mx_dell_job(&tost->jobs, node->pid);
    }
    else if (WIFEXITED(status)) {
        status = WEXITSTATUS(status);
        tost->out = 0;
        mx_dell_job(&tost->jobs, node->pid);
    }
    tcsetpgrp(0, shell_pid);
    tcsetpgrp(1, shell_pid);
}

static t_jobs *new_node(t_jobs *i) {
    t_jobs *node = (t_jobs *)malloc(sizeof(t_jobs));

    node->flag = i->flag;
    node->name = mx_strdup(i->name);
    node->next = NULL;
    node->num = i->num;
    node->pid = i->pid;
    node->step = i->step;
    return node;
}

static void free_new_node(t_jobs **i) {
    t_jobs *node = *i;
    free(node->name);
    free(node);
}

static bool check_first_error(char *src, int *result) {
    if (src[0] == '%')
        return true;
    *result = 1;
    mx_print_fg(NULL, 2, src);
    return false;
}

static char *find_num_name(char *src1, int fl) {
    char *s = NULL;

    for (int i = 0; src1[i]; i++) {
        if ((isalpha(src1[0]) && fl == 0)
        || (isdigit(src1[0]) && fl == 1))
            s = mx_join_char(s, src1[i]);
        else
            break;
    }
    return s;
}

static int find_step_job(int fl, t_ost *tost, int pos, char *src) {
    if (!tost->jobs)
        return -1;
    int num = -1;
    t_jobs *i = NULL;

    for (i = tost->jobs; i; i = i->next) {
        if ((fl == 0 && i->flag == '-')
            || (fl == 1 && i->flag == '+'))
            num = i->step;
        else if (fl == 2 && i->num == pos)
            num = i->step;
        else if (fl == 3 && strncmp(i->name, src, mx_strlen(src)) == 0)
            num = i->step;
    }
    return num;
}

static int find_job(char *src, t_ost *tost, int *result) {
    char *i = src;
    char *s = NULL;
    int num = -1;

    for (; *i == '%'; i += 1);
    if (i[0] == '+' || i[0] == '\0') {
        num = find_step_job(1, tost, 0, NULL);
        if (num == -1) {
            mx_print_fg(NULL, 3, NULL);
            *result = 1;
        }
    }
    else if (i[0] == '-') {
        num = find_step_job(0, tost, 0, NULL);
        if (num == -1) {
            mx_print_fg(NULL, 4, NULL);
            *result = 1;
        }
    }
    else if (isalpha(i[0])) {
        s = find_num_name(i, 0);
        num = find_step_job(3, tost, 0, s);
        free(s);
        if (num == -1) {
            mx_print_fg(NULL, 1, i);
            *result = 1;
        }
    }
    else if (isdigit(i[0])) {
        s = find_num_name(i, 1);
        num = find_step_job(2, tost, atoi(s), NULL);
        free(s);
        if (num == -1) {
            mx_print_fg(NULL, 2, i);
            *result = 1;
        }
    }
    else {
        mx_print_fg(NULL, 2, i);
        *result = 1;
    }
    return num;
}

void mx_fg_start(char *src, t_ost *tost, int *result) {
    if (check_first_error(src, result)) {
        int step = find_job(src, tost, result);
        t_jobs *i = NULL;

        if (step != -1) {
            for (i = tost->jobs; i->step != step; i = i->next);
            t_jobs *node = new_node(i);
            
            mx_print_fg(i, 0, NULL);
            mx_continue(tost, node);
            free_new_node(&node);
        }
    }
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

int mx_true(char **argv, t_ost *tost) {
    if (tost->env)
        printf("%s\n", argv[0]);
    return 0;
}

int mx_false(char **argv, t_ost *tost) {
    if (tost->env)
        printf("%s\n", argv[0]);
    return 1;
}

int mx_jobs(char **argv, t_ost *tost) {
    if (mx_lenn_mass(argv) > 1) {
        mx_printerr("jobs: too many arguments\n");
        return 1;
    }
    if (tost->jobs) {
        for (t_jobs *i = tost->jobs; i; i = i->next) {
            mx_printstr("[");
            mx_printint(i->num);
            mx_printstr("]");
            mx_printstr("  ");
            mx_printint(i->pid);
            mx_printstr("  ");
            mx_printchar(i->flag);
            mx_long_print(" ", "suspended", "  ", i->name);
            mx_printchar('\n');
        }
    }
    return 0;
}

static bool chech_pid(t_jobs *list, int pid) {
    for (t_jobs *i = list; i; i = i->next)
        if (i->pid == pid)
            return true;
    return false;
}

int mx_kill(char **argv, t_ost *tost) {
    int result = 0;

    for (int i = 1; argv[i]; i++) {
        int num = mx_atoi(argv[i]);

        if (chech_pid(tost->jobs, num)) {
            if (kill(num, SIGKILL) == -1) {
                perror("kill: ");
                result = 1;
            }
            else {
                wait(NULL);
                mx_dell_job(&tost->jobs, num);
                if (!tost->jobs)
                    tost->out = 0;
            }
        }
        else {
            mx_printerr("kill: illegal pid: ");
            mx_printint(num);
            mx_printerr("\n");
            result = 1;
        }
    }
    return result;
}

int mx_change(char **argv, t_ost *tost) {
    if (mx_lenn_mass(argv) != 2) {
        mx_printerr("change [useful / user / fun / normal]\n");
        return 1;
    }
    if (mx_strcmp(argv[1], "normal") == 0)
        tost->flag = 0;
    else if (mx_strcmp(argv[1], "useful") == 0)
        tost->flag = 1;
    else if (mx_strcmp(argv[1], "user") == 0)
        tost->flag = 2;
    else if (mx_strcmp(argv[1], "fun") == 0)
        tost->flag = 3;
    else if (mx_strcmp(argv[1], "secret") == 0) {
        mx_printstr("\033[1;31moh shit!\033[0m\n");
        sleep(2);
        mx_printstr("\033[1;32mIt is a secret prompt, motherfucer!!!\033[0m\n");
        sleep(2);
        mx_printstr("\033[1;33mGOD, it is the best terminal that I've ever seen)\033[0m\n");
        sleep(2);
        mx_printstr("\033[1;34mGuys that have creted it should be smart!\033[0m\n");
        sleep(2);
        mx_printstr("\033[1;35mIlyha, kogda nachnesh stroky hendlit'???\033[0m\n");
        sleep(2);
        mx_printstr("\033[1;36mKak i obaschal\033[0m\n");
        tost->flag = 4;
    }
    else {
        mx_printerr("there is not any parameters such as: ");
        mx_printerr(argv[1]);
        mx_printerr("\n");
        return 1;
    }
    return 0;
}

int mx_next(char **argv, t_ost *tost) {
    if (mx_lenn_mass(argv) != 1) {
        mx_printerr("bro, just write: \"next\"\n");
        return 1;
    }
    else if (tost->flag != 3) {
        mx_printerr("you have to use \"change fun\" command before\n");
        return 1;    
    }
    tost->prompt = tost->prompt->next;
    return 0;
}
