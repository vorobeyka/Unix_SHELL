#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <sys/types.h>
#include <uuid/uuid.h>
#include <grp.h>
#include <sys/xattr.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/acl.h>
#include <sys/uio.h>
#include <malloc.h>

#include <signal.h>
#include <setjmp.h>

#include <fcntl.h>
#include <termios.h>

#include "inc/parse_header.h"
#include "libmx/inc/libmx.h"

// #include <readline/readline.h>
// #include <readline/history.h>

typedef struct s_built {
    char *home_catalog;
    char *curctlg;
    char *oldctlg;
    char *command_line;
    char **commands;
    char *envpwd;
    char *lpwd;
    char *ppwd;
    char **own_env;
    int full_path;
    int old_cat;
    int status;
}              t_built;

typedef struct s_history {
    char *data;
    struct s_history *next;
    struct s_history *previos;
}              t_history;

typedef struct s_support {
    char *name;
    char *line;
    struct s_support *next; 
}              t_support;

typedef struct s_jobs {
    int pid;
    int num;
    int step;
    char flag;
    char *name;
    struct s_jobs *next;
}              t_jobs;

typedef struct s_be_happy {
    char *emoji;
    char *phrase;
    char *code;
    struct s_be_happy *next;
}              t_be_happy;

typedef struct s_ost {
    int flag;
    int out;
    int last;
    int status;
    char *path;
    char **env;
    char **vars;
    struct termios old;
    struct termios read;
    struct s_redi *redi;
    struct s_jobs *jobs;
    struct s_history *history;
    struct s_support *support;
    struct s_be_happy *prompt;
    struct s_built *built;
}              t_ost;

// New terminal
void mx_new_terminal(t_ost *tost);

// History
int mx_lenn_list(t_history *list);
void mx_push_list(t_history **list, char *data);
void mx_dell_list(t_history **list);
void mx_dell_full(t_history *list);

// Help
int mx_lenn_mass(char **arr);
void mx_long_print(char *src1, char *src2, char *src3, char *src4);
char *mx_super_join(char *src1, char *src2, int fl);
char *mx_join_char(char *src, char c);
char **mx_trim_split(char *command, char c);
void mx_long_error_print(char *src1, char *src2, char *src3, char *src4);
bool mx_cech_end(void);
int mx_reverse_index(char *src, char s);
char *mx_add(char *str, char c, int *i);
char **mx_resize_one(char **s, char *add);

// Built-in
bool mx_check_builtin(char *name);
int mx_builtin(char **argv, t_ost *tost);
int mx_export(char **argv, t_ost *tost);
int mx_unset(char **argv, t_ost *tost);
int mx_fg(char **argv, t_ost *tost);
int mx_exit(char **argv, t_ost *tost);
int mx_env(char **argv, t_ost *tost);
int mx_cd(char **argv, t_ost *tost);
int mx_pwd(char **argv, t_ost *tost);
int mx_which(char **argv, t_ost *tost);
int mx_echo(char **argv, t_ost *tost);
int mx_true(char **argv, t_ost *tost);
int mx_false(char **argv, t_ost *tost);
int mx_jobs(char **argv, t_ost *tost);
int mx_kill(char **argv, t_ost *tost);
int mx_change(char **argv, t_ost *tost);
int mx_next(char **argv, t_ost *tost);

// Char
void mx_add_char(t_history *node, char ch, int position);
void mx_dell_char(t_history *node, int position);

// Read
void mx_pluss_char(t_history *node, char *ch, int *position);
void mx_minus_char(t_history *node, int *position);
char *mx_ush_read(t_history *old_node, t_ost *tost);

// Ush loop
void mx_ush_loop(t_ost *tost);
int mx_start(t_ost *tost, char *com_line);

// Ush exe
void mx_pipe_rec(char **arr, int lenn, t_ost *tost);
int mx_ush_execute(char *command, t_ost *tost);
void mx_exe(char **command, t_ost *tost);

// Signal
void mx_set_signals(void);

// Support
int mx_pre_support(t_ost *tost, char *line);
int mx_support(t_ost *tost, char *name, char *command);
int mx_command(t_support *list, char *line, t_ost *tost);
int mx_find_comamnd(char **arr);
bool mx_chech_support(char *line);
bool mx_check_command(t_support *list, char *line);
bool mx_pre_check(char *src, t_ost *tost, int fl);
char *mx_clean_subsh(char *src);

// Jobs
void mx_add_job(char *name, int pid, t_jobs **list);
void mx_dell_job(t_jobs **list, int pid);

// Pars
char **mx_parse_all(char *line, t_ost *tost);
char **mx_parse_pipes_and_tz(char *line, char c);

#endif
