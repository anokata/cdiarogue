#include <stdio.h>
#include <assert.h>
#include "app.h"

void test_() {
    G g = g_init("./maps/loc1");
    printf("you@ %d:%d\n", g->player->x, g->player->y);
    free_g(g);
}

void test_savefile_load() {
    G g = g_init("./maps/loc1");
    load_savefile(g, "./save/test.save");
    free_g(g);
}

int main() {
    test_();
    test_savefile_load();
}
