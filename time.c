#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

void delay_test(char *params[]) {
    (void)params;
    time_t lst = time(NULL);
    time_t now = time(NULL);
    while (1) {
        usleep(1000000);
        now = time(NULL);
        printf("1 second sleep. Delta: %ld \tFrom begin: %ld \n", now - lst, now);
        lst = now;
    }
}

void diff(char *params[]) {
    (void)params;
    printf("%s\n", params[0]);
    /* Get now time */
    time_t now = time(NULL);
    printf("Now is: %s\n", ctime(&now));
    /* Convert input to time, "2Y - add 2 years" */
    /* calc diff in weeks days months*/
    /* out */
}

typedef void (*CmdFunc)(char *params[]);

typedef struct Command {
    char *name;
    CmdFunc f;
} Command;

Command commands[] = {
    {"delay", delay_test},
    {"diff", diff},
};

CmdFunc get_command(char *name) {
    int cmd_count = sizeof(commands) / sizeof(Command);
    for (int i = 0; i < cmd_count; i++) {
        if (strcmp(name, commands[i].name) == 0) {
            return commands[i].f;
        }
    }
    return commands[0].f;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("commands:\n\tdelay\n\tdiff - time diff from this\n");
        return 0;
    }
    get_command(argv[1])(argv + 2);
}
