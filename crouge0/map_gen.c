#include "map_gen.h"

Map make_map(int width, int heigth) { // OK
    Map map = malloc(sizeof(struct Map));
    char *data = calloc(width * heigth, 1);
    map->data = data;
    map->width = width;
    map->heigth = heigth;
    for (int i=0; i < heigth; i++) {
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

Map gen_map(int width, int heigth) {
    time_t t;
    srand((unsigned) time(&t));
    map_chars_count = strlen(map_chars);

    Map map = make_map(width, heigth);

    for (int i=0; i < heigth; i++) {
        for (int j=0; j < width; j++) {
            map->data[i * width + j] = rand_char();
        }
    }
    return map;
}

string map_to2d(Map map) { // OK
    int nwidth = map->width + 1;
    string map2d = malloc(1 + map->heigth * nwidth);
    memset(map2d, 0, map->heigth * nwidth);

    for (int i=0; i < map->heigth; i++) {
        for (int j=0; j < map->width; j++) {
            map2d[i * nwidth + j] = map->data[i * map->width + j];
        }
        if (i < map->heigth - 1)
            map2d[(i + 1) * nwidth - 1] = '\n';
    }
    return map2d;
}

int get_map_size(Map map) {
    return map->width * map->heigth;
}

void print_map(Map map) { // OK
    string m2;
    m2 = map_to2d(map);
    DEBUG_PRINT("w:%d h:%d\n", map->width, map->heigth);
    printf("%s\n", m2);
    free(m2);
}

// to viewable with newlines, from viewable/editable in vi
// mode for open view map in editor in tmp file, edit, save and convert to map.

int out_map(char *filename, int width, int heigth) {
    Map map = gen_map(width, heigth);
    FILE *file = fopen(filename, "w");

    char *buf = malloc(100);
    sprintf(buf, "%d\n%d\n", map->width, map->heigth);
    fwrite(buf, strlen(buf), 1, file);
    free(buf);

    fwrite(map->data, get_map_size(map), 1, file);
    fclose(file);

    DEBUG_PRINT("Map:\n");
    print_map(map);
    DEBUG_PRINT("Saved to %s\n", filename);
    free_map(map);
    return 0;
}

Map load_map(string filename) { // OK
    Map map = 0;
    FILE *file = fopen(filename, "r");
    int width = 0;
    int heigth = 0;

    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    width = fget_int_line(file);
    heigth = fget_int_line(file);
    DEBUG_PRINT("Loading map with w:%d h:%d\n", width, heigth);

    map = make_map(width, heigth);

    read = getline(&line, &len, file);
    memcpy(map->data, line, read - 1);
    /* print_map(map); */

    if (line)
        free(line);
    fclose(file);
    return map;
}

