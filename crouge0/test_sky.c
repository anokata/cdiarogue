#include "sky_view.h"
#include <stdio.h>

void test_sky() {
    printf(DEFAULT);
    printf("sky:\n");
    Sky s = make_sky();
    char *str = sky_draw_string(s);
    printf("%s\n", str);
    /* for (int i = 0; i < 255; i += 4) { */
    /*     printf("\033[48;2;0;0;%dm ", i); */
    /* } */

    free(str);
    free_sky(s);
    printf(DEFAULT);
}

void test_time_step(int t) {
    char *s = sky_steps2time(t);
    printf("%d =\t %s\n", t, s);
    /* free(s); */
}

void test_time_prn() {
    test_time_step(0);
    test_time_step(1);
    test_time_step(2);
    test_time_step(60);
    test_time_step(61);
    test_time_step(126);
    test_time_step(3662);
}

int main(void) {
    test_time_prn();
}
