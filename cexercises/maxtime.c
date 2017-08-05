#include <stdio.h>
#include <time.h>
#include <limits.h>

int main() {
    printf("Time (9)\n");

    time_t current;
    current = time(NULL);
    printf("sizeof time_t = %ld bytes.\n", sizeof(time_t));
    printf("Current epoch seconds: %ld\n", current);

    time_t maxtime = ULONG_MAX;
    time_t diff = (long int)difftime(maxtime, current);
    printf("Max epoch seconds: %ld\n", maxtime);
    printf("Difference between max time and current in seconds = %.0lf\n", difftime(maxtime, current));
    printf("Difference between max time and current in seconds = %ld\n", diff);

    printf("Now: %s", ctime(&current));
    printf("Diff with max: %s", ctime(&diff));
}
