#include "../inc/header.h"

static t_be_happy *create_happiness(char *emoji,
                                    char *phrase, char *code) {
    t_be_happy *node = (t_be_happy *)malloc(sizeof(t_be_happy));

    node->emoji = mx_strdup(emoji);
    node->phrase = mx_strdup(phrase);
    node->code = mx_strdup(code);
    node->next = NULL;
    return node;
}

static void push_me(char *emoji, char *phrase,
                    char *code, t_be_happy **list) {
    t_be_happy *node = create_happiness(emoji, phrase, code);
    t_be_happy *i = NULL;

    if (!*list) {
        *list = node;
        return ;
    }
    i = *list;
    for (; i->next; i = i->next);
    i->next = node;
}

static void close_it(t_be_happy **list) {
    t_be_happy *i = *list;
    
    for (; i->next; i = i->next);
    i->next = *list;
}

static void push_part(t_be_happy **list) {
    char octupus[5] = {0xF0, 0x9F, 0x90, 0x99, '\0'};
    char alian[5] = {0xF0, 0x9F, 0x91, 0xBD, '\0'};
    char imp[5] = {0xF0, 0x9F, 0x91, 0xBF, '\0'};
    char gost[5] = {0xF0, 0x9F, 0x91, 0xBB, '\0'};
    char poo[5] = {0xF0, 0x9F, 0x92, 0xA9, '\0'};

    push_me(octupus, " octupus ", "\033[1;31m", list);
    push_me(alian, " alian ", "\033[1;32m", list);
    push_me(imp, " imp ", "\033[1;33m", list);
    push_me(gost, " gost ", "\033[1;34m", list);
    push_me(poo, " poo ", "\033[1;35m", list);
}

void mx_feel_it(t_be_happy **list) {
    char unicorn[5] = {0xF0, 0x9F, 0xA6, 0x84, '\0'};
    char boom[5] = {0xF0, 0x9F, 0xA4, 0xAF, '\0'};
    char mouth[5] = {0xF0, 0x9F, 0xA4, 0xAE, '\0'};
    char love[5] = {0xF0, 0x9F, 0xA5, 0xB0, '\0'};
    char dragon[5] = {0xF0, 0x9F, 0x90, 0x89, '\0'};

    push_part(list);
    push_me(unicorn, " unicorn ", "\033[1;31m", list);
    push_me(boom, " boom ", "\033[1;32m", list);
    push_me(mouth, " mouth ", "\033[1;33m", list);
    push_me(love, " love ", "\033[1;34m", list);
    push_me(dragon, " dragon ", "\033[1;36m", list);
    close_it(list);
}
