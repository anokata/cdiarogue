#include "lib/ccurses.h"
#include "sky_view.h"
#include <string.h>
// Sun position 6--22
// day night color
// weather cond
// Simple KISS
// 1. without colors?
// base color
// fixed width or not?
//

Sky make_sky() {
    Sky s = malloc(sizeof(struct Sky));
    s->hour = 12;
    s->width = 10;
    return s;
}


void free_sky(Sky s) {
    free(s);
}

char *sky_draw_string(Sky s) {
    char *str = NULL;
    asprintf(&str, "[%s%s(*)%s%s]", BGBLUED, RED, BGBLUE_END, DEFAULT);
    return str;
}

char *sky_steps2time(int steps) {
    /* One step = 1 sec */
    int hours = steps / (60 * 60);
    int minutes = (steps / 60) - hours * 60;
    int seconds = steps - hours * 60 * 60 - minutes * 60;
    static char buf[BUFSIZE];
    snprintf(buf, BUFSIZE, "%02d:%02d:%02d", hours, minutes, seconds);
    return buf;
}
