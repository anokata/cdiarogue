#include "sky_view.h"
#include <stdio.h>

int main(void) {
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
