#include "map_gen.h"

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("(%d) Usage: mapgen FILENAME WIDTH HEIGHT\n", argc);
        string file = "/tmp/build/test.map";
        out_map(file, 20, 10);
        TileMap m = load_tile_map(file);
        print_tile_map(m);
        free_tile_map(m);
        return 0;
    }
    char *filename = argv[1];
    int width = atoi(argv[2]);
    int height = atoi(argv[3]);
    printf("Creating map: %s %d/%d\n", filename, width, height);

    Map map = gen_map(width, height);
    save_map(map, filename);

    TileMap m = load_tile_map(filename);
    print_tile_map(m);
    free_tile_map(m);
}

