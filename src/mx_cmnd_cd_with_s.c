#include "../inc/header.h"

int mx_cmnd_cd_with_s(t_built *u, char *ctlg) {
    char tmp[512] = { 0 };

    ctlg = mx_to_ctlg(ctlg);
    if (readlink(ctlg, tmp, 512) > 0) {
        mx_long_error_print("cd: not a directory: ", ctlg, "\n", NULL);
        mx_strdel(&ctlg);
        return 1;
    }
    return mx_cmnd_do_cd(u, ctlg);
}
