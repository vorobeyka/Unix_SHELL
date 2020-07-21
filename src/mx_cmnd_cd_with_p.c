#include "../inc/header.h"

int mx_cmnd_cd_with_p(t_built *u, char *ctlg) {
    if (!chdir(ctlg)) {
        mx_free_mass(ctlg, NULL, NULL, NULL);
        ctlg = getcwd(NULL, 0);
    }
    return mx_cmnd_do_cd(u, ctlg);
}
