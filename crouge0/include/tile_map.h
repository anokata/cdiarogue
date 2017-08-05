#pragma once

#define DEBUG

#include "lib/ccurses.h"
#include "util.h"

typedef struct Tile {
    Color color;
    char c;
} Tile;

typedef struct MapTile {
    Tile tile;
    int x;
    int y;
} MapTile;

typedef struct TileMap {
    Tile *tiles;
    int width;
    int heigth;
} *TileMap;

typedef struct Viewport {
    int width;
    int heigth;
    int cx;
    int cy;
} Viewport;

typedef void (*TileFunc)(Tile *tile, int x, int y);

TileMap make_tile_map(int width, int heigth);
TileMap load_tile_map(string filename);
TileMap load_global_tmap();
void free_tile_map(TileMap map);

void print_tile_map(TileMap map);
string tilemap_to2d(TileMap map);
Tile *tile_at(TileMap map, int x, int y);
void draw_map(TileMap map, Viewport *v);
void foreach_tile(TileMap map, TileFunc f);
