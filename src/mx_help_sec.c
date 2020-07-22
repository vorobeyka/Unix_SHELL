#include "../inc/header.h"

void mx_long_error_print(char *src1, char *src2,
                         char *src3, char *src4) {
    if (src1)
        write(2, src1, mx_strlen(src1));
    if (src2)
        write(2, src2, mx_strlen(src2));
    if (src3)
        write(2, src3, mx_strlen(src3));
    if (src4)
        write(2, src4, mx_strlen(src4));

}

int mx_lenn_mass(char **arr) {
    int count = 0;

    for (int i = 0; arr[i]; i++)
        count++;
    return count;
}

char **mx_trim_split(char *command, char c) {
    char *trim_command = mx_strtrim(command);
    char **argv = mx_strsplit(trim_command, c);

    free(trim_command);
    for (int i = 0; argv[i]; i++) {
        char *v = argv[i];

        argv[i] = mx_strtrim(argv[i]);
        free(v);
        v = NULL;
    }
    return argv;
}

char *mx_add(char *str, char c, int *i) {
    if (c == '\0') {
        return str;
    }
    int len = str ? mx_strlen(str) : 0;
    char *rez = (char *) malloc((len + 2) * sizeof(char));

    rez = mx_strncpy(rez, str, len);
    if (str)
        free(str);
    rez[len + 1] = '\0';
    rez[len] = c;
    *i += 1;
    return rez;
}

char **mx_resize_one(char **s, char *add) {
    if (!add)
        return s;
    int len = mx_len_of_array(s) + 2;
    char **rez = (char **) malloc(len * sizeof(char *));

    if (!s) {
        rez[0] = mx_strdup(add);
        rez[1] = NULL;
    }
    else {
        for (int i = 0; s[i]; i++) {
            rez[i] = mx_strdup(s[i]);
            free(s[i]);
        }
        if(s)
            free(s);
        rez[len - 2] = mx_strdup(add);
        rez[len - 1] = NULL;
    }
    return rez;
}


