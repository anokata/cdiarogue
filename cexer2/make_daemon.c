#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>

void fork_fun(void(*daemon_func)(), void(*parent_func)(pid_t pid)) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork: ");
        return;
    }
    if (pid == 0) { // child
        daemon_func();
        exit(0);
    } else { // parent
        parent_func(pid);
        exit(0);
    }
}

void parent_func(pid_t pid) {
    printf("%d\n", pid);
}

void signal_handler(int s) {
    //printf("signal %d\n", s);
    exit(0);
}

void daemon_func() {
    //signal
    struct sigaction new_action, old_action;
    new_action.sa_handler = signal_handler;
    sigemptyset(&new_action.sa_mask);
    new_action.sa_flags = 0;
    //sigaction(SIGURG, NULL, &old_action);
    sigaction(SIGURG, &new_action, NULL);

    chdir("/");
    if (setsid() == -1) {
        perror("setsid: ");
        return;
    }
    fclose(stdin);
    fclose(stderr);
    fclose(stdout);
    while(1) usleep(500000);
}

void demonize() {
    fork_fun(daemon_func, parent_func);
}

int main(int argc, char* argv[], char* envp[]) {
    demonize();
    return 0;
}
