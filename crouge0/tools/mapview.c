#include <stdio.h>
#include "tile_map.h"

int main(int argc, char* argv[]) {
    UNUSED(argc);
    UNUSED(argv);
    DEBUG_PRINT("%d:", argc);
    DEBUG_PRINT("%s\n", argv[0]);
    TileMap l = load_tile_map("../maps/map_1_1");
    print_tile_map(l);
    free_tile_map(l);
}
