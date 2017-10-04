#include "lightmap.h"
#include "tile_map.h"
#include <stdio.h>
#include <assert.h>

void test_map() {
    TileMap g = load_global_tmap("maps/loc1");

    LightMap lm = lightmap_new(g->width, g->height);

    /* print_tile_map(l); */
    GList *it = g->objects;
    while (it) {
        object_print(it->data);
        it = g_list_next(it);
    }
    free_global_map(g);
    lightmap_free(lm);
}

int main() {
    test_map();
}
