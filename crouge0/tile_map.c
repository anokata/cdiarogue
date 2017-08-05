#include "tile_map.h"

TileMap make_tile_map(int width, int heigth) {
    TileMap map = malloc(sizeof(struct TileMap));
    Tile *tiles = calloc(width * heigth, sizeof(struct Tile));
    map->tiles = tiles;
    map->width = width;
    map->heigth = heigth;
    for (int i=0; i < heigth; i++) {
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
    Tile *tile = map->tiles + y * map->width + x;
    return tile;
}

string tilemap_to2d(TileMap map) {
    int nwidth = map->width + 1;
    string map2d = malloc(1 + map->heigth * nwidth);
    memset(map2d, 0, map->heigth * nwidth);
memset(map2d, '-', map->heigth * nwidth - 1);

    for (int i=0; i < map->heigth; i++) {
        for (int j=0; j < map->width; j++) {
            map2d[i * nwidth + j] = tile_at(map, j, i)->c;
        }
        if (i < map->heigth - 1)
            map2d[(i + 1) * nwidth - 1] = '\n';
    }
    return map2d;
}

void print_tile_map(TileMap map) {
    string m2;
    m2 = tilemap_to2d(map);
    DEBUG_PRINT("w:%d h:%d\n", map->width, map->heigth);
    printf("%s\n", m2);
    free(m2);
}

void copy_map2tiles(TileMap map, char *line, int len, int offset) {
    for (int i = 0; i < len; i++) {
        map->tiles[i + offset].c = line[i];
    }
}

void apply_color(TileMap map, char c, int color_index) {
    for (int y = 0; y < map->heigth; ++y) {
        for (int x = 0; x < map->heigth; ++x) {
            if (tile_at(map, x, y)->c == c) {
                tile_at(map, x, y)->color = cc_get_color_by_id(color_index);
            }
        }
    }
}

void load_colors(TileMap map, FILE *file) {
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, file)) != -1) {
        char c = line[0];
        int color_index = atoi(line + 1);
        apply_color(map, c, color_index);
        printf("%c:%d\n", c, color_index);
    }  
    if (line) free(line);
}

TileMap load_tile_map(string filename) {
    TileMap map = 0;
    FILE *file = fopen(filename, "r");
    int width = 0;
    int heigth = 0;

    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    int mode = fget_int_line(file);
    width = fget_int_line(file);
    heigth = fget_int_line(file);
    DEBUG_PRINT("Loading tile map with w:%d h:%d\n", width, heigth);

    map = make_tile_map(width, heigth);

    if (mode == 0) { // Local map
        read = getline(&line, &len, file);
        copy_map2tiles(map, line, read - 1, 0);
        load_colors(map, file);
    } else if (mode == 1) { // Global map by line
        for (int y = 0; y < heigth; y++) {
            read = getline(&line, &len, file);
            copy_map2tiles(map, line, read - 1, y * width);
        }
    }
    /* print_tile_map(map); */

    if (line)
        free(line);
    fclose(file);
    return map;
}

void _copy_tileloc2glob(TileMap gmap, TileMap lmap, int offset) {
    for (int i = 0; i < lmap->heigth; i++) {
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
    global_map = make_tile_map(local_width * local_map_width, local_height * local_map_height);
    DEBUG_PRINT("Global tile map with w:%d h:%d\n", global_map->width, global_map->heigth);
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
    int top = v.cy - v.heigth / 2;
    int left = v.cx - v.width / 2;
    int bottom = (v.cy + v.heigth / 2);// % map->heigth + 0;
    int right = (v.cx + v.width / 2) % map->width + 0;
    top = (top < 0) ? 0 : top;
    left = (left < 0) ? 0 : left;

    for (int y = top; y < bottom; ++y) {
        for (int x = left; x < right; ++x) {
            f(tile_at(map, x, y), x - left, y - top);
        }
    }
}

void foreach_tile(TileMap map, TileFunc f) {
    for (int y = 0; y < map->heigth; ++y) {
        for (int x = 0; x < map->width; ++x) {
            f(tile_at(map, x, y), x, y);
        }
    }
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
