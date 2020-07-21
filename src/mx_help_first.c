#include "../inc/header.h"

void mx_free_mass(void *v1, void *v2, void *v3, void *v4) {
    if (v1)
        free(v1);
    if (v2)
        free(v2);
    if (v3)
        free(v3);
    if (v4)
        free(v4);
}

int *mx_create_integer_massive(int size) {
    int *rez = (int *) malloc(size * sizeof(int));

    for (int i = 0; i < size; i++)
        rez[i] = 0;
    return rez;
}

char **mx_array_copy_strs(char **array) {
    int len = mx_len_of_array(array);
    char **rez = (char **) malloc((len + 1) * sizeof(char *));

    for (int i = 0; array[i]; i++)
        rez[i] = mx_strdup(array[i]);
    rez[len] = NULL;
    return rez;
}

int mx_error_return(char *msg, int value) {
    mx_printerr(msg);
    return value;
}

char *mx_full_path(char *str) {
    char *rez = NULL;
    int len = mx_strlen(str);

    if (mx_reverse_index(str, '/') == 0 && len != 1)
        len--;
    for (int i = len - 1; str[i] != '/'; i--)
        len--;
    rez = (char *) malloc((len + 1) * sizeof(char));
    strncpy(rez, str, len);
    rez[len] = '\0';
    return rez;
}

