#include <stdio.h>
#include "tile_map.h"

int main(int argc, char* argv[]) {
    UNUSED(argc);
    char *map_name = argv[1];
    if (map_name) {
    
    } else {
        map_name = "./maps/map_1_1";
    }

    // Read from stdin
    char *map_data = NULL;
    size_t len = 0;
    getline(&map_data, &len, stdin);

    TileMap l = load_tile_map(map_name);

    copy_map2tiles(l, map_data, strlen(map_data), 0);

    print_tile_map(l);
    // Save! TODO
    free_tile_map(l);
    free(map_data);
    map_data = NULL;
}
