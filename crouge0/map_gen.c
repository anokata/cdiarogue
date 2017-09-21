#include "map_gen.h"

Map make_map(int width, int height) { // OK 
    Map map = malloc(sizeof(struct Map));
    char *data = calloc(width * height, 1);
    map->data = data;
    map->width = width;
    map->height = height;
    for (int i=0; i < height; i++) {
        for (int j=0; j < width; j++) {
            data[i * width + j] = ' ';
        }
    }
    return map;
}

void free_map(Map map) { // OK
    free(map->data);
    free(map);
}

static const char map_chars[] = ".......ox";
int map_chars_count = sizeof(map_chars);
// TODO chars and probabilities

char rand_char() {
    return map_chars[rand() % map_chars_count];
}

char map_char_at(TileMap map, int x, int y) {
    return map->tiles[y * map->width + x].c;
}

int map_neighbours(Map map, Point p) {
    int n = 0;
    UNUSED(map);
    UNUSED(p);
    // TODO
    return n;
}

void gen_map_step(Map map) {
    // TODO
    UNUSED(map);
}

Map gen_map(int width, int height) {
    time_t t;
    srand((unsigned) time(&t));
    map_chars_count = strlen(map_chars);

    Map map = make_map(width, height);

    for (int i=0; i < height; i++) {
        for (int j=0; j < width; j++) {
            map->data[i * width + j] = rand_char();
        }
    }
    return map;
}

string map_to2d(Map map) { // OK
    int nwidth = map->width + 1;
    string map2d = malloc(1 + map->height * nwidth);
    memset(map2d, 0, map->height * nwidth);

    for (int i=0; i < map->height; i++) {
        for (int j=0; j < map->width; j++) {
            map2d[i * nwidth + j] = map->data[i * map->width + j];
        }
        if (i < map->height - 1)
            map2d[(i + 1) * nwidth - 1] = '\n';
    }
    return map2d;
}

int get_map_size(Map map) {
    return map->width * map->height;
}

void print_map(Map map) { // OK DEPRECATED
    string m2;
    m2 = map_to2d(map);
    DEBUG_PRINT("w:%d h:%d\n", map->width, map->height);
    printf("%s\n", m2);
    free(m2);
}

// to viewable with newlines, from viewable/editable in vi
// mode for open view map in editor in tmp file, edit, save and convert to map.

int out_map(char *filename, int width, int height) {
    Map map = gen_map(width, height);
    save_map(map, filename);
    return 0;
}

void save_map(Map map, char *filename) {
    FILE *file = fopen(filename, "w");

    char *buf = malloc(100);
    sprintf(buf, "mode:%d\nwidth:%d\nheight:%d\nmap:", 0, map->width, map->height); 
    fwrite(buf, strlen(buf), 1, file);
    free(buf);

    fwrite(map->data, get_map_size(map), 1, file);
    fwrite("\n", 1, 1, file);
    // TODO tiles_count colors etc
    fclose(file);

    DEBUG_PRINT("Map:\n");
    print_map(map);
    DEBUG_PRINT("Saved to %s\n", filename);
    free_map(map);
}
