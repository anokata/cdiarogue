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

    m = load_tile_map(filename);
    print_tile_map(m);

    map = tilemap_convert2map(m);
    print_map(map);

    memset(map->data, '*', 30);
    print_map(map);

    free_tile_map(m);
    m = map_convert2tilemap(map);
    print_tile_map(m);
    save_tilemap(m, "/tmp/out");
    print_tile_map(m);
    free_tile_map(m);

    free_tile_map(m);
    free_map(map);


    m = load_tile_map(filename);
    save_tilemap(m, "/tmp/out2");
    free_tile_map(m);
    m = load_tile_map("/tmp/out2");
    print_tile_map(m);
    free_tile_map(m);
}

void test_gen_rooms() {
    int width = 40;
    int height = 20;
    DEBUG_PRINT("\n***Creating map gen rooms\n");
    Map map = gen_map_rooms_split(width, height);
    print_map(map);
    free_map(map);
}

void test_gen() {
    int width = 40;
    int height = 20;
    DEBUG_PRINT("\n***Creating map gen\n");
    Map map = gen_map(width, height);
    gen_proc_cave(map);
    gen_map_invert(map);
    print_map(map);

    TileMap m = map_convert2tilemap(map);
    m->tiles_file = "map_1_1.tiles";
    save_tilemap(m, "/tmp/out");

    free_map(map);
}

void test_name() {
    printf("%s\n%s\n", gen_mapname(), gen_path("TEST"));
}

int main() {
    /* test(); */
    /* test_gen_rooms(); */
    /* test_gen(); */
    /* test_name(); */
    char *name = gen_location(20, 30, "town", "lvl2", "lvl1");
    free(name);
}

