#include "tile_map.h"
#include "config_parser.h"

TileMap make_tile_map(int width, int height) {
    TileMap map = malloc(sizeof(struct TileMap));
    Tile *tiles = calloc(width * height, sizeof(struct Tile));
    map->tiles = tiles;
    map->width = width;
    map->height = height;
    for (int i=0; i < height; i++) {
        for (int j=0; j < width; j++) {
            tiles[i * width + j].c = ' ';
        }
    }
    return map;
}

void free_tile_map(TileMap map) {
    free(map->tiles);
    free(map);
}

Tile *tile_at(TileMap map, int x, int y) {
    if (x > map->width) exit(1);
    //debug_file_log_format("x %d  y %d.\n", x, y);
    Tile *tile = map->tiles + y * map->width + x;
    return tile;
}

string tilemap_to2d(TileMap map) {
    int nwidth = map->width + 1;
    string map2d = malloc(1 + map->height * nwidth);
    memset(map2d, 0, map->height * nwidth);
memset(map2d, '-', map->height * nwidth - 1);

    for (int i=0; i < map->height; i++) {
        for (int j=0; j < map->width; j++) {
            map2d[i * nwidth + j] = tile_at(map, j, i)->c;
        }
        if (i < map->height - 1)
            map2d[(i + 1) * nwidth - 1] = '\n';
    }
    return map2d;
}

void print_tile_map(TileMap map) {
    string m2;
    m2 = tilemap_to2d(map);
    DEBUG_PRINT("w:%d h:%d\n", map->width, map->height);
    printf("%s\n", m2);
    free(m2);
}

void copy_map2tiles(TileMap map, char *line, int len, int offset) {
    for (int i = 0; i < len; i++) {
        map->tiles[i + offset].c = line[i];
    }
}

void apply_color(TileMap map, char c, int color_index) {
    for (int y = 0; y < map->height; ++y) {
        for (int x = 0; x < map->height; ++x) {
            if (tile_at(map, x, y)->c == c) {
                tile_at(map, x, y)->color = cc_get_color_by_id(color_index);
            }
        }
    }
}

void _set_tile_passable(Tile *tile, int x, int y, void *data) {
    bool passable = (bool)data;
    tile->passable = passable;
    UNUSED(x);
    UNUSED(y);
}

char _get_tile_opt(GHashTable *config, int i, char *params) {
    char *values = g_hash_table_lookup(config, params);
    return values[i - 1];
}


char _get_tile_char(GHashTable *config, int i) {
    return _get_tile_opt(config, i, "tiles");
    // for other method: return g_hash_table_lookup(config, tile_char_name);
}

char _get_tile_color(GHashTable *config, int i) {
    return _get_tile_opt(config, i, "colrs") - '0';
}

bool _get_tile_passable(GHashTable *config, int i) {
    return _get_tile_opt(config, i, "passb") == 't';
}

void load_colors(TileMap map, GHashTable *config) {
    int tiles_count = atoi(g_hash_table_lookup(config, "tiles_count"));
    for (int i = 1; i <= tiles_count; i++) {
        char tile_char = _get_tile_char(config, i);
        int color_index = _get_tile_color(config, i);
        bool passable = _get_tile_passable(config, i);
        printf("Tile#%d = '%c' color#%d pass:%s\n", i, 
                tile_char, 
                color_index, 
                passable ? "true" : "false");
        apply_color(map, tile_char, color_index);
        foreach_tile_set(map, _set_tile_passable, (void*)passable, tile_char);
    }
}

TileMap load_tile_map(string filename) {
    TileMap map = NULL;
    GHashTable *config = parse_file(filename);
    /* how global? */
    int mode = atoi(g_hash_table_lookup(config, "mode"));
    int width = atoi(g_hash_table_lookup(config, "width"));
    int height = atoi(g_hash_table_lookup(config, "height"));
    char *map_data = g_hash_table_lookup(config, "map");
    DEBUG_PRINT("Loading tile map with w:%d h:%d\n", width, height);

    map = make_tile_map(width, height);

    if (mode == 0) { 
        /* Local map */
        copy_map2tiles(map, map_data, strlen(map_data) - 1, 0);
        load_colors(map, config); 
    } else if (mode == 1) { 
        /* Global map by line */
        copy_map2tiles(map, map_data, strlen(map_data) - 1, 0);
        // TODO
        //load_description(map); 
        //load_wmap_data(world_filename);
    }
    print_tile_map(map);
    g_hash_table_destroy(config);

    return map;
}

void _copy_tileloc2glob(TileMap gmap, TileMap lmap, int offset) {
    for (int i = 0; i < lmap->height; i++) {
        memcpy(gmap->tiles + offset + i * gmap->width, 
               lmap->tiles + i * lmap->width, lmap->width * sizeof(struct Tile));
    }
}

TileMap load_global_tmap() {
    TileMap global_map;
    int local_width = 2;
    int local_height = 2;
    int local_map_width = 6;
    int local_map_height = 3;
    /* Read params from map info file */
    GHashTable *config = parse_file("./maps/info");
    local_map_width = atoi(g_hash_table_lookup(config, "map_width"));
    local_map_height = atoi(g_hash_table_lookup(config, "map_height"));
    local_width = atoi(g_hash_table_lookup(config, "x_segments"));
    local_height = atoi(g_hash_table_lookup(config, "y_segments"));
    g_hash_table_destroy(config);

    global_map = make_tile_map(local_width * local_map_width, local_height * local_map_height);
    DEBUG_PRINT("Global tile map with w:%d h:%d\n", global_map->width, global_map->height);
    string mapname_format = "maps/map_%i_%i";
    char mapname[100];
    int block_height = local_map_width * local_width * local_map_height;

    for (int i = 1; i <= local_height; i++) {
        for (int j = 1; j <= local_width; j++) {
            sprintf(mapname, mapname_format, i, j);
            printf("name: %s\n", mapname);
			TileMap lmap = load_tile_map(mapname);
            _copy_tileloc2glob(global_map, lmap, 
                    local_map_width * (j - 1) + (i - 1) * block_height);
			free_tile_map(lmap);
        }
    }
    /* print_tile_map(global_map); */
    
    return global_map;
}

void foreach_tile_viewport(TileMap map, TileFunc f, Viewport v) {
    int top = v.cy - v.height / 2;
    int left = v.cx - v.width / 2;
    int bottom = min((v.cy + v.height / 2), map->height);
    int right = min((v.cx + v.width / 2), map->width);
    top = (top < 0) ? 0 : top;
    left = (left < 0) ? 0 : left;

    /* debug_file_log_format("viewport cx %d  cy %d.\n", v.cx, v.cy); */
    /* debug_file_log_format("viewport w %d  h %d. map w h %d %d\n", v.width, v.height, map->width, map->height); */
    /* debug_file_log_format("foreach t %d  l %d.\n", top, left); */
    /* debug_file_log_format("foreach b %d  r %d.\n", bottom, right); */
    for (int y = top; y < bottom; ++y) {
        for (int x = left; x < right; ++x) {
            f(tile_at(map, x, y), 
              x - left + v.display_left, 
              y - top + v.display_top);
        }
    }
}

void foreach_tile(TileMap map, TileFunc f) {
    for (int y = 0; y < map->height; ++y) {
        for (int x = 0; x < map->width; ++x) {
            f(tile_at(map, x, y), x, y);
        }
    }
}

void foreach_tile_set(TileMap map, TileDataFunc f, void *data, char tile_char) {
    for (int y = 0; y < map->height; ++y) {
        for (int x = 0; x < map->width; ++x) {
            Tile *tile = tile_at(map, x, y);
            if (tile_char == '\0' || tile_char == tile->c) {
                f(tile, x, y, data);
            }
        }
    }
}

bool is_passable(TileMap map, int x, int y) {
    return tile_at(map, x, y)->passable;
}

void draw_tile(Tile *tile, int x, int y) {
    cc_putxy(tile->c, tile->color, x, y);
}

void draw_map(TileMap map, Viewport *v) {
    // draw by tile
    /* foreach_tile(map, draw_tile); */
    /* Viewport ve = {6, 6, 0, 0}; */
    foreach_tile_viewport(map, draw_tile, *v);
    /* string m2; */
    /* m2 = tilemap_to2d(map); // store to g? */
    /* cc_print(m2, cd_yellow); */
    /* free(m2); */
}

void viewport_move_left(Viewport *v, TileMap map) {
    if (v->cx == 0) return;
    if (! is_passable(map, (v->cx - 1), v->cy)) return;
    v->cx--;
}

void viewport_move_right(Viewport *v, TileMap map) {
    if (! is_passable(map, (v->cx + 1), v->cy)) return;
    v->cx++;
}

void viewport_move_up(Viewport *v, TileMap map) {
    if (v->cy == 0) return;
    if (! is_passable(map, v->cx, (v->cy - 1))) return;
    v->cy--;
}

void viewport_move_down(Viewport *v, TileMap map) {
    if (! is_passable(map, v->cx, (v->cy + 1))) return;
    v->cy++;
}
