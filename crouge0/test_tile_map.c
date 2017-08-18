#include "tile_map.h"

int main() {
    TileMap m = make_tile_map(2, 3);
    print_tile_map(m);
    /* TileMap l = load_tile_map("maps/map_1_1"); */
    TileMap l = load_tile_map("maps/map_0_1");
    print_tile_map(l);
    free_tile_map(l);
    free_tile_map(m);

    TileMap g = load_global_tmap();
    print_tile_map(g);
    free_tile_map(g);
}
