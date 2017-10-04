#include "lightmap.h"
#include "tile_map.h"
#include "lightcalc.h"
#include <stdio.h>
#include <assert.h>

void test_map() {
    TileMap g = load_global_tmap("maps/loc1");

    LightMap lm = lightmap_new(g->width, g->height);
    lightmap_free(lm);
    lm = NULL;

    map_lightning(g, 5, 5);

    for (int x = 0; x < g->width; x++) {
        for (int y = 0; y < g->height; y++) {
            if (g->lightmap[y * g->width + x]) {
                printf("%c", '.');
            } else {
                printf("%c", '`');
            }
        }
        printf("%c", '\n');
    }

    /* print_tile_map(l); */
    GList *it = g->objects;
    while (it) {
        object_print(it->data);
        it = g_list_next(it);
    }
    free_global_map(g);
    g = NULL;
}

int main() {
    test_map();
}
