#include "header.h"

static t_be_happy *create_happiness(char *emoji, char *phrase, char *code) {
    t_be_happy *node = (t_be_happy *)malloc(sizeof(t_be_happy));

    node->emoji = mx_strdup(emoji);
    node->phrase = mx_strdup(phrase);
    node->code = mx_strdup(code);
    node->next = NULL;
    return node;
}

static void push_me(char *emoji, char *phrase, char *code, t_be_happy **list) {
    t_be_happy *node = create_happiness(emoji, phrase, code);

    if (!*list) {
        *list = node;
        return ;
    }
    t_be_happy *i = *list;

    for (; i->next; i = i->next);
    i->next = node;
}

static void close_it(t_be_happy **list) {
    t_be_happy *i = *list;
    
    for (; i->next; i = i->next);
    i->next = *list;
}

void mx_feel_it(t_be_happy **list) {
    char unicorn[] = {0xF0, 0x9F, 0xA6, 0x84};
    char boom[] =    {0xF0, 0x9F, 0xA4, 0xAF};
    char mouth[] =   {0xF0, 0x9F, 0xA4, 0xAE};
    char love[] =    {0xF0, 0x9F, 0xA5, 0xB0};
    char scuid[] =   {0xF0, 0x9F, 0xA6, 0x91};
    char dragon[] =  {0xF0, 0x9F, 0x90, 0x89};
    char octupus[] = {0xF0, 0x9F, 0x90, 0x99};
    char alian[] =   {0xF0, 0x9F, 0x91, 0xBD};
    char imp[] =     {0xF0, 0x9F, 0x91, 0xBF};
    char gost[] =    {0xF0, 0x9F, 0x91, 0xBB};
    char poo[] =     {0xF0, 0x9F, 0x92, 0xA9};
    char sream[] =   {0xF0, 0x9F, 0x98, 0xB1};
    push_me(unicorn, " unicorn ", "\033[1;31m", list);
    push_me(boom, " boom ", "\033[1;32m", list);
    push_me(mouth, " mouth ", "\033[1;33m", list);
    push_me(love, " love ", "\033[1;34m", list);
    push_me(scuid, " scuid ", "\033[1;35m", list);
    push_me(dragon, " dragon ", "\033[1;36m", list);
    push_me(octupus, " octupus ", "\033[1;31m", list);
    push_me(alian, " alian ", "\033[1;32m", list);
    push_me(imp, " imp ", "\033[1;33m", list);
    push_me(gost, " gost ", "\033[1;34m", list);
    push_me(poo, " poo ", "\033[1;35m", list);
    push_me(sream, " sream ", "\033[1;36m", list);
    close_it(list);
}

static char *create_buff(void) {
    char *s = mx_strnew(0);
    char *v = NULL;
    char ch;

    while (read(0, &ch, 1) > 0) {
        v = s;
        s = mx_realloc(s, mx_strlen(v) + 1);
        s[mx_strlen(v)] = ch;
        free(v);
    }
    v = s;
    s = mx_realloc(s, mx_strlen(v) + 1);
    s[mx_strlen(v)] = '\0';
    free(v);
    return s;
}

static void shell_level(void) {
    char *old = getenv("SHLVL");
    char *new = mx_itoa(atoi(old) + 1);

    if (setenv("SHLVL", new, 1) == -1) {
        free(new);
        perror("ush: setenv");
        exit(1);
    }
    free(new);
}

t_built *push_built() {
    t_built *rez = (t_built *) malloc(1 * sizeof(t_built));

    rez->home_catalog = getenv("HOME");
    rez->command_line = NULL;
    rez->commands = NULL;
    rez->curctlg = getenv("PWD");
    rez->old_cat = 0;
    rez->oldctlg = getenv("OLDPWD");
    rez->status = 1;
    return rez;
}

void mx_new_terminal(t_ost *tost) {
    extern char **environ;

    tost->built = push_built();
    tost->env = environ;
    tost->out = 0;
    tost->last = 0;
    tost->status = 0;
    tost->flag = 0;
    tost->path = getenv("_");
    tost->jobs = NULL;
    tost->history = NULL;
    tost->support = NULL;
    tost->prompt = NULL;
    tost->redi = NULL;
    mx_feel_it(&tost->prompt);
    shell_level();
    if (isatty(0)) {
        tcgetattr(0, &tost->old);
        tcgetattr(0, &tost->read);
        tost->read.c_lflag &= ~(ICANON | ECHO | ISIG);
        tost->read.c_cc[VMIN] = 1;
    }
    else {
        char *s = create_buff();
        int result = mx_start(tost, s);
        
        free(s);
        exit(result);
    }
}
