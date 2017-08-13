#include <stdio.h>
#include <unistd.h>
#include <time.h>

int main() {
    time_t lst = time(NULL);
    time_t now = time(NULL);
    while (1) {
        usleep(1000000);
        now = time(NULL);
        printf("1 second sleep. Delta: %d \tFrom begin: %d \n", now - lst, now);
        lst = now;
    }
}
