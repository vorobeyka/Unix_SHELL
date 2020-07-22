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
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/acl.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <sys/xattr.h>
#include <uuid/uuid.h>
#include <grp.h>
#include <time.h>
#include <signal.h>
#include <setjmp.h>
#include <pwd.h>
#include <termios.h>
#include <errno.h>
#include <assert.h>
#include <limits.h>

#include "../libmx/inc/libmx.h"

typedef struct s_cd {
    char *phys;
    char *link;
    char *new;
    char *tmp;
    char *for_oldpwd;
}              t_cd;

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
    struct s_cd *cd;
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
    int trim;
    int flag;
    int exit_slesh;
    int out;
    int status;
    int error;
    bool kostil;
    bool kostil_x2;
    char *path;
    char **env;
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
bool mx_cech_end(void);
void mx_long_print(char *src1, char *src2, char *src3, char *src4);
void mx_long_error_print(char *src1, char *src2, char *src3, char *src4);
void mx_free_mass(void *v1, void *v2, void *v3, void *v4);
int mx_lenn_mass(char **arr);
int mx_reverse_index(char *src, char s);
int mx_error_return(char *msg, int value);
int *mx_create_integer_massive(int size);
char *mx_super_join(char *src1, char *src2, int fl);
char *mx_join_char(char *src, char c);
char *mx_full_path(char *str);
char *mx_add(char *str, char c, int *i);
char **mx_resize_one(char **s, char *add);
char **mx_trim_split(char *command, char c);
char **mx_array_copy_strs(char **array);

// Built-in
t_jobs *mx_new_node(t_jobs *i);
void mx_find_exe(char *str, int flag, int *result);
void mx_find_support(char *str, int flag,
                         int *result, t_ost *tost);
void mx_find_built_in(char *str, int flag, int *result);
void mx_print_fg(t_jobs *node, int fl, char *src);
void mx_continue(t_ost *tost, t_jobs *node);
void mx_free_new_node(t_jobs **i);
void mx_chenge_flag_list(t_jobs *list);
void mx_feel_it(t_be_happy **list);
void mx_fg_start(char *src, t_ost *tost, int *result);
bool mx_check_builtin(char *name);
bool mx_check_first_error(char *src, int *result);
int mx_which(char **argv, t_ost *tost);
int mx_find_step_job(int fl, t_ost *tost, int pos, char *src);
int mx_true(char **argv, t_ost *tost);
int mx_false(char **argv, t_ost *tost);
int mx_jobs(char **argv, t_ost *tost);
int mx_change(char **argv, t_ost *tost);
int mx_next(char **argv, t_ost *tost);
int mx_exit(char **argv, t_ost *tost);
int mx_builtin(char **argv, t_ost *tost);
int mx_export(char **argv, t_ost *tost);
int mx_cmnd_export_recurs(char **cmnd, int i, int *err);
int mx_unset(char **argv, t_ost *tost);
int mx_fg(char **argv, t_ost *tost);
int mx_env(char **argv, t_ost *tost);
int mx_cd(char **argv, t_ost *tost);
int mx_cmnd_cd_flags(t_built *u);
int mx_flagscd(char *str);
int mx_cmnd_cd_with_s(t_built *u, char *ctlg);
int mx_cmnd_cd_with_p(t_built *u, char *ctlg);
int mx_cmnd_do_cd(t_built *u, char *ctlg);
char *mx_cmnd_cd_dots(char *ctlg);
char *mx_to_ctlg(char *str);
int mx_pwd(char **argv, t_ost *tost);
int mx_echo(char **argv, t_ost *tost);
char *mx_cmnd_echo_trans_x2(char *src);
char mx_last_char(char *str);
int mx_kill(char **argv, t_ost *tost);
char *mx_find_num_name(char *src1, int fl);
char *mx_cmnd_cd_new_ctlg(t_built *u);
char *mx_correct_ctlg(t_built *u, char *str);
char *mx_cmnd_echo_trans(char *src);
char *mx_shift_one(char *add, char *s, char c, int *i);
char **mx_add_env(char **shift);
char **mx_option_loop(char **arr, int *flag);
bool mx_is_space(char c);
bool mx_is_last_char(char c);
bool mx_is_one_symb(char c, char *str, int *flag);

// Char
void mx_add_char(t_history *node, char ch, int position);
void mx_dell_char(t_history *node, int position);

// Read
void mx_dell_space(char *src, int position);
void mx_print_data(char *src);
void mx_minus_back_char(t_history *node, int *position);
void mx_fir_part(char ch[3], int *position, t_history **node);
void mx_shift_list(t_history **node, int flag, int *position);
void mx_sec_part(t_ost **tost, t_history **node, char **s, int flag);
void mx_pluss_char(t_history *node, char *ch, int *position);
void mx_minus_char(t_history *node, int *position);
bool mx_check_enter_exit(char *line);
char *mx_ush_read(t_history *old_node, t_ost *tost);
t_history *mx_buffer_list(t_history **node);

// Ush loop
void mx_ush_loop(t_ost *tost);
void mx_change_last(t_history **node, char *line);
void mx_clean_history(t_history *node);
bool mx_first_built_check(char *src);
int mx_start(t_ost *tost, char *com_line);

// Ush exe
void mx_pipe_rec(char **arr, int lenn, t_ost *tost);
void mx_exe(char **command, t_ost *tost, char *str, int fl);
int mx_ush_execute(char *command, t_ost *tost);

// Signal
void mx_set_signals(void);

// Support
int mx_pre_support(t_ost *tost, char *line);
int mx_support(t_ost *tost, char *name, char *command, char *line);
int mx_command(t_support *list, char *line, t_ost *tost);
bool mx_chech_support(char **arr);
bool mx_check_valid_supp(char *line);
bool mx_check_command(t_support *list, char **arr);
bool mx_pre_check(char *src, t_ost *tost, int fl);
char *mx_clean_subsh(char *src);
char *mx_command_find(char *str);

// Jobs
void mx_add_job(char *name, int pid, t_jobs **list);
void mx_dell_job(t_jobs **list, int pid);

// Pars
char **mx_parse_all(char *line, t_ost *tost);
char **mx_parse_pipes_and_tz(char *line, char c);
char *mx_open_quotes(char *s, t_ost *tost);
char *mx_parse_slash(char *str, char *s, int *i);
void mx_shift_spaces(char *str, int *i);
void mx_find_delim(char *str, char delim, int *i);
void mx_last_delim(char *str, char delim, int *i);
char *mx_add_quotes(char *str, char *s, int *i, t_ost *tost);
char *mx_parse_command_x2(char *str, char *s, int *i, t_ost *tost);
char *mx_to_not_odd(char *rez, char *s, int *i, t_ost *tost);
char *mx_parse_odd(char *s, int *i);
int mx_check_quote(char c);
int mx_check_delims(char c);
char *mx_add_with_hook(char *s, int *i, t_ost *tost);
char *mx_add_with_laters(char *s, int *i);
char *mx_split_quotes_x2(char *add, char *str, int *i, int *err);
char *mx_split_quotes(char *add, char *str, int *err, int *i);
char *mx_split_hooks_baks(char *add, char *str, int *i, int *err);
char *mx_parse_baks(char *s, int *i, t_ost *tost);
char *mx_prog_out(char *line, t_ost *tost);
char **mx_split_commands(char *str, t_ost *tost);
char *mx_parse_not_odd_slash(char *add, char *s, int *i);
char mx_add_white(char c);
char **mx_resize_array(char **s, char **add);
char *mx_cut_subsh(char *str);
int mx_if_else(char **command, int i, t_ost *tost);
int pre_builtin(char *src, t_ost *tost);
void mx_env_part(char **argv, t_ost *tost);
void mx_run_shell(char **argv, t_ost *tost, char *str, int fl);
void mx_env_set_grp(char **argv, t_ost *tost, pid_t pid);

#endif
