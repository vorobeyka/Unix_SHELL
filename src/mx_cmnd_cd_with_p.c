#include "../inc/header.h"

int mx_cmnd_cd_with_p(t_built *u, char *ctlg) {
    // char tmp[512] = { 0 };
    // char *new_ctlg = NULL;    
    if (!chdir(ctlg)) {
        ctlg = getcwd(NULL, 0);
    }
    return mx_cmnd_do_cd(u, ctlg);
    // if (readlink(ctlg, tmp, 512) > 0) {
    //     if (u->full_path)
    //         new_ctlg = mx_full_path(u->commands[2]);
    //     else
    //         new_ctlg = mx_strjoin(u->curctlg, "/");
    //     free(ctlg);
    //     return mx_cmnd_do_cd(u, mx_super_join(new_ctlg, tmp, 1));
    // }
    // else
    //     return mx_cmnd_do_cd(u, ctlg);
}
