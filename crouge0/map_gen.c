#include "map_gen.h"
#include <stdbool.h>
#include "item.h"
#include "actor.h"

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

struct IntPair gen_get_free_space(Map map) {
    struct IntPair p= {0, 0};
    p.a = rand() % map->width;
    p.b = rand() % map->height;
    int limit = 1000;
    while (!(char_at(map, p.a, p.b) == floor) && limit) {
        p.a = rand() % map->width;
        p.b = rand() % map->height;
        limit--;
    }
    return p;
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

char *gen_mapname() {
    char name[] = "            ";
    char chars[] = "0123456789ABCDEFGHJKLMNOPQRSUVWXYZabcdefghjklmnopqrsuvwxyz";
    size_t chars_count = strlen(chars);
    for (size_t i = 0; i < strlen(name); i++) {
        name[i] = chars[rand() % chars_count];
    }
    return strdup(name);
}

char *gen_path(char *suffix, char* name) {
    char map_path[BUFSIZE];
    snprintf(map_path, BUFSIZE, "%s%s", name, suffix);
    return strdup(map_path);
}

void location_save_data(char *filename, int width, int height, 
        char* actors_file, char* items_file, char* objects_file,
        char* map_prefix) {
    FILE *fout = fopen(filename, "w");
    char buf[BUFSIZE];
    snprintf(buf, BUFSIZE, 
            "x_segments:1\n"
            "y_segments:1\n"
            "map_width:%d\n"
            "map_height:%d\n"
            "actors:%s\n"
            "items:%s\n"
            "map_name_prefix:%s\n"
            "objects:%s\n"
            , width, height, actors_file, items_file, map_prefix, objects_file);

    fwrite(buf, strlen(buf), 1, fout);
    fwrite("\n", 1, 1, fout);
    fclose(fout);
}

void gen_location(int width, int height) {
    char *common_tiles_file = "map_1_1.tiles";
    char *items_file_lvl1 = "maps/lvl1.items";
    char *actors_file_lvl1 = "maps/lvl1.actors";
    char *mapname = gen_mapname();
    char map_path_buf[BUFSIZE];
    snprintf(map_path_buf, BUFSIZE, "save/%s_1_1.map", mapname);
    char *map_path = strdup(map_path_buf);

    char *items_path = gen_path(".items", mapname);
    char *actors_path = gen_path(".actors", mapname);
    char *objects_path = gen_path(".objs", mapname);
    char *loc_path = gen_path(".loc", mapname);

    Map map = gen_map(width, height);
    gen_proc_cave(map);
    gen_map_invert(map);

    TileMap m = map_convert2tilemap(map);
    m->tiles_file = strdup(common_tiles_file);
    save_tilemap(m, map_path);

    free(map_path);

    /* gen items from set of lvl */
    int items_take_count = (width * height) / 20;
    // load items file for specific lvl
    Items items = items_load(items_file_lvl1);
    int items_count = g_list_length(items);
    // take N random items: clone add
    for (int i = 0; i < items_take_count; i++) {
    // take item
        Item item = g_list_nth_data(items, rand() % items_count);
        item = item_clone(item);
    // place item
        struct IntPair p = gen_get_free_space(map);
        item->x = p.a;
        item->y = p.b;
    // add item
        item_add(&m->items, item);
    // save items
        char items_full_path[BUFSIZE];
        snprintf(items_full_path, BUFSIZE, "save/%s", items_path);
        items_save(items_full_path, m->items);
    }

    /* gen exit and enter */
    struct IntPair enter_point = gen_get_nine_space(map);
    struct IntPair exit_point = gen_get_nine_space(map);
    // todo: add objects

    /* gen mobs */
    int actors_take_count = (width * height) / 20;
    Actors actors = actors_load(actors_file_lvl1);
    int actors_count = g_list_length(actors);
    for (int i = 0; i < actors_take_count; i++) {
        Actor actor = g_list_nth_data(actors, rand() % actors_count);
        actor = actor_clone(actor);
        struct IntPair p = gen_get_free_space(map);
        actor->x = p.a;
        actor->y = p.b;
        actor_add(&m->actors, actor);
        char actors_full_path[BUFSIZE];
        snprintf(actors_full_path, BUFSIZE, "save/%s", actors_path);
        actors_save(actors_full_path, m->actors);
    }

    /* location */
    // save location
    char location_full_path[BUFSIZE];
    snprintf(location_full_path, BUFSIZE, "save/%s", loc_path);
    location_save_data(location_full_path, width, height, actors_path,
            items_path, objects_path, mapname);

    items_free(&items);
    /* items_free(&m->items); */
    free_tile_map(m);
    free(loc_path);
    free(objects_path);
    free(actors_path);
    free(items_path);
    free(mapname);
    free_map(map);
}
