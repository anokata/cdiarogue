#include "map_gen.h"

Map make_map(int width, int height) { // OK 
    Map map = malloc(sizeof(struct Map));
    char *data = calloc(width * height, 1);
    memset(data, '.', width * height);
    map->data = data;
    map->width = width;
    map->height = height;
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

static const char wall = '#';
static const char floor = '.';

void map_horizont_line(Map map, int y) {
    for (int i = 0; i < map->width; i++) {
        map->data[y * map->width + i] = wall;
    }
}

void map_vetical_line(Map map, int x) {
    for (int i = 0; i < map->height; i++) {
        map->data[i * map->width + x] = wall;
    }
}

// Test it
void map_vetical_line_to(Map map, int x, int y) {
    for (int i = y; i < map->height && map->data[i * map->width + x] == floor; i++) {
        map->data[i * map->width + x] = wall;
    }
}

Map gen_map_rooms_split(int width, int height) {
    time_t t;
    srand((unsigned) time(&t));
    map_chars_count = strlen(map_chars);

    Map map = make_map(width, height);

    // TODO save old, not do close?
    for (int i = 0; i < 1; i++) {
        int x = rand() % width;
        int y = rand() % height;
        int hv = rand() % 2;
        if (hv) {
            map_horizont_line(map, y);
        } else {
            /* map_vetical_line(map, x); */
            map_vetical_line_to(map, x, y);
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

Map tilemap_convert2map(TileMap tmap) {
    Map map = make_map(tmap->width, tmap->height);
    for (int i=0; i < tmap->height; i++) {
        for (int j=0; j < tmap->width; j++) {
            Tile t = tmap->tiles[i * tmap->width + j];
            map->data[i * tmap->width + j] = t.c;
        }
    }
    return map;
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

/* void save_tilemap(TileMap map, char *filename) { */
/* } */
