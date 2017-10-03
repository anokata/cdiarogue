#include "tile_map.h"

void test_tmap_create_load() {
    TileMap m = make_tile_map(2, 3);
    print_tile_map(m);
    TileMap l = load_tile_map("maps/map_1_1");
    print_tile_map(l);
    free_tile_map(l);
    free_tile_map(m);

    TileMap g = load_global_tmap("maps/loc1");
    print_tile_map(g);
    free_tile_map(g);
}

void test_load_loc() {
    TileMap g = load_global_tmap("maps/loc1");
    /* print_tile_map(l); */
    free_global_map(g);
}

int main() {
    /* test_tmap_create_load(); */
    test_load_loc();
}
