#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void my_sig_int(int x) {
    static int u1 = 0;
    static int u2 = 0;
    if (x == SIGUSR1) u1++;
    if (x == SIGUSR2) u2++;
    if (x == SIGTERM) {
        printf("%d %d\n", u1, u2);
        exit(0);
    }
}

int main() {
    signal(SIGUSR1, my_sig_int);
    signal(SIGUSR2, my_sig_int);
    signal(SIGTERM, my_sig_int);
    while(1);
    return 0;
}
