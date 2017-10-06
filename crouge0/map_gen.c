#include "map_gen.h"
#include <stdbool.h>


char char_at(Map map, int x, int y);

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

static const char map_chars[] = ".##";
static const char wall = '#';
static const char floor = '.';
int map_chars_count = sizeof(map_chars);
// TODO chars and probabilities

char rand_char() {
    return map_chars[rand() % map_chars_count];
}

char map_char_at(TileMap map, int x, int y) {
    return map->tiles[y * map->width + x].c;
}

int map_neighbours(Map map, int a, int b) {
    int n = 0;
    for (int x = -1; x < 2; x++) {
        for (int y = -1; y < 2; y++) {
            if (char_at(map, y + a, x + b) != floor) {
                n++;
            }
        }
    }
    return n;
}

bool gen_map_step(Map map, int threshold) {
    bool changed = false;
    for (int i=0; i < map->height; i++) {
        for (int j=0; j < map->width; j++) {
            int n = map_neighbours(map, j, i);
            if (n < threshold) {
                map->data[i * map->width + j] = floor;
                changed = true;
            }
        }
    }
    return changed;
}

void gen_proc_cave(Map map) {
    for (int i = 0; i < 3; i++) {
        gen_map_step(map, 5);
    }
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

void gen_map_floodfill(Map map, int x, int y) {
    UNUSED(map);
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (char_at(map, j + x, i + y) == floor) {

            } else {
            }
        }
    }
    /* TODO */
}

void gen_map_invert(Map map) {
    for (int i=0; i < map->height; i++) {
        for (int j=0; j < map->width; j++) {
            if (map->data[i * map->width + j] == wall) {
                map->data[i * map->width + j] = floor;
            } else {
                map->data[i * map->width + j] = wall;
            }
        }
    }
}


char char_at(Map map, int x, int y) {
    /* printf("char@ %d %d\n", x, y); */
    if (x >= map->width || x < 0) return floor;
    if (y >= map->height || y < 0) return floor;
    return map->data[y * map->width + x];
}

char *pchar_at(Map map, struct IntPair p) {
    return &map->data[p.b * map->width + p.a];
}

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

void map_horizont_line_to(Map map, int x, int y) {
    for (int i = x; (i < map->width) && (char_at(map, i, y) == floor); i++) {
        map->data[y * map->width + i] = wall;
    }
    for (int i = x - 1; (i >= 0) && (char_at(map, i, y) == floor); i--) {
        map->data[y * map->width + i] = wall;
    }
}

void map_vetical_line_to(Map map, int x, int y) {
    for (int i = y; i < map->height && map->data[i * map->width + x] == floor; i++) {
        map->data[i * map->width + x] = wall;
    }
    for (int i = y - 1; i >= 0 && map->data[i * map->width + x] == floor; i--) {
        map->data[i * map->width + x] = wall;
    }
}

bool gen_is_9_space(Map map, int x, int y) {
    return (
        (char_at(map, x, y)       == floor) && 
        (char_at(map, x, y-1)     == floor) && 
        (char_at(map, x, y+1)     == floor) && 
        (char_at(map, x-1, y)     == floor) && 
        (char_at(map, x-1, y-1)   == floor) && 
        (char_at(map, x-1, y+1)   == floor) && 
        (char_at(map, x+1, y)     == floor) && 
        (char_at(map, x+1, y-1)   == floor) && 
        (char_at(map, x+1, y+1)   == floor)
    );
}

struct IntPair gen_get_nine_space(Map map) {
    struct IntPair p= {0, 0};
    p.a = rand() % map->width;
    p.b = rand() % map->height;
    int limit = 1000;
    while (!gen_is_9_space(map, p.a, p.b) && limit) {
        p.a = rand() % map->width;
        p.b = rand() % map->height;
        DEBUG_PRINT("try %d %d\n", p.a, p.b);
        limit--;
    }
    return p;
}

Map gen_map_rooms_split(int width, int height) {
    time_t t;
    srand((unsigned) time(&t));
    map_chars_count = strlen(map_chars);

    Map map = make_map(width, height);

    // TODO save old, not do close?
    int x = rand() % width;
    int y = rand() % height;
    for (int i = 0; i < 20; i++) {
        /* while (char_at(map, x, y) != floor) { */
        struct IntPair point = gen_get_nine_space(map);
        x = point.a;
        y = point.b;
        int hv = rand() % 2;
        if (hv) {
            map_horizont_line_to(map, x, y);
        } else {
            map_vetical_line_to(map, x, y);
        }
    }

    struct IntPair point = gen_get_nine_space(map);
    *pchar_at(map, point) = '>';

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

TileMap map_convert2tilemap(Map map) {
    TileMap tmap = make_tile_map(map->width, map->height);
    for (int i=0; i < map->height; i++) {
        for (int j=0; j < map->width; j++) {
            tmap->tiles[i * map->width + j].c = map->data[i * map->width + j];
        }
    }
    return tmap;
}

void save_map(Map map, char *filename) {
    FILE *file = fopen(filename, "w");

    char *buf = malloc(100);
    sprintf(buf, "mode:%d\nwidth:%d\nheight:%d\nmap:", 0, map->width, map->height); 
    fwrite(buf, strlen(buf), 1, file);
    free(buf);

    fwrite(map->data, get_map_size(map), 1, file);
    fwrite("\n", 1, 1, file);
    fclose(file);

    DEBUG_PRINT("Map:\n");
    /* print_map(map); */
    DEBUG_PRINT("Saved to %s\n", filename);
    free_map(map); //TODO don't save
}

void save_tilemap(TileMap map, char *filename) {
    FILE *file = fopen(filename, "w");

    char *buf = malloc(100);
    sprintf(buf, "mode:%d\nwidth:%d\nheight:%d\nmap:", 0, map->width, map->height); 
    fwrite(buf, strlen(buf), 1, file);

    Map m = tilemap_convert2map(map);
    fwrite(m->data, get_map_size(m), 1, file);
    fwrite("\n", 1, 1, file);

    if (map->tiles_file) {
        sprintf(buf, "tiles_file:%s\n", map->tiles_file); 
        fwrite(buf, strlen(buf), 1, file);
    }

    fclose(file);
    free(buf);
    free_map(m);
}
