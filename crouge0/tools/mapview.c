#include <stdio.h>
#include "tile_map.h"

int main(int argc, char* argv[]) {
    UNUSED(argc);
    UNUSED(argv);
    DEBUG_PRINT("%d:", argc);
    DEBUG_PRINT("%s\n", argv[0]);
    char *map_name = argv[1];
    if (map_name) {
    
    } else {
        map_name = "../maps/map_1_1";
    }

    TileMap l = load_tile_map(map_name);
    print_tile_map(l);
    free_tile_map(l);
}
