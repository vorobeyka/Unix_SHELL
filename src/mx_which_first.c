#include "../inc/header.h"

static void mx_part_find_exe(char *str, int flag,
                          char *split, int *result) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(split);
    if (dir) {
        while ((entry = readdir(dir)) != NULL) {
            if (mx_strcmp(entry->d_name, str) == 0) {
                *result = 0;
                if (flag != 2)
                    printf("%s/%s\n", split, str);
            }
        }
        closedir(dir);
    }
}

void mx_find_exe(char *str, int flag, int *result) {
    char *path = getenv("PATH");

    if (path) {
        char **split = mx_trim_split(path, ':');

        for (int i = 0; split[i]; i++)
            mx_part_find_exe(str, flag, split[i], result);
        mx_del_strarr(&split);
    }
}

void mx_find_support(char *str, int flag,
                         int *result, t_ost *tost) {
    for (t_support *i = tost->support; i; i = i->next)
        if (mx_strcmp(i->name, str) == 0) {
                *result = 0;
                if (flag != 2) {
                    printf("%s () {\n", str);
                    char **split = mx_trim_split(i->line, ';');

                    for (int j = 0; split[j]; j++)
                        printf("\t%s\n", split[j]);
                    mx_del_strarr(&split);
                    printf("}\n");
                }
            }
}

void mx_find_built_in(char *str, int flag, int *result) {
    char *builtin[] = {"env", "cd", "pwd", "which", "echo", "export",
                       "unset", "fg", "exit", "true", "false", "jobs",
                       "kill", "change", "next", NULL};

    for (int i = 0; builtin[i]; i++)
        if (mx_strcmp(builtin[i], str) == 0) {
            *result = 0;
            if (flag != 2)
                printf("%s: ush built-in command\n", str);
        }
}
