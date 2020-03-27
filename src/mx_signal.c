#include "header.h"

void mx_set_signals(void) {
    signal(SIGINT, SIG_IGN); // Ctrl + C
    signal(SIGTSTP, SIG_IGN); // Ctrl + Z
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
}
