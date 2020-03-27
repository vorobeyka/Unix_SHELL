#include "header.h"

static void mx_ush(t_ost *tost) {
    mx_set_signals();
    mx_ush_loop(tost);
    mx_dell_full(tost->history);
}

// atoi isalpa isdigit strncmp
int main() {
    t_ost tost;
    mx_new_terminal(&tost);
    mx_ush(&tost);

    // system("leaks -q ush");
    return 0;
}
