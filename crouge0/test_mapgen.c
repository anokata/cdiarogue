#include "map_gen.h"

void test() {
    char *filename = "/tmp/build/test.map";
    int width = 20;
    int height = 10;
    printf("Creating map: %s %d/%d\n", filename, width, height);

    Map map = gen_map(width, height);
    save_map(map, filename);

    TileMap m = load_tile_map(filename);
    print_tile_map(m);
    free_tile_map(m);

    printf("\n***Creating map gen rooms\n");
    map = gen_map_rooms_split(width, height);
    print_map(map);
    free_map(map);
}

int main() {
    test();
}

