#pragma once
#include "lib/ccurses.h"
#include "util.h"
#include "tile_map.h"

typedef struct Map {
    char *data;
    int width;
    int height;
} *Map;

typedef struct Point {
    int x;
    int y;
} Point;

typedef struct WorldMapRegionDescriptor { //wmrd
    char c;
    Color color;
    string description;
} WorldMapRegionDescriptor;

typedef struct WorldMap {
    TileMap map;
    WorldMapRegionDescriptor *metadata;
} *WorldMap;


typedef struct GlobalMapTile { //?
    char c;
    Color color;
} GlobalMapTile;
