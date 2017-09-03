#pragma once
#include "lib/ccurses.h"
#include "util.h"

typedef struct Map {
    char *data;
    int width;
    int height;
} *Map;

typedef struct WorldMapRegionDescriptor { //wmrd
    char c;
    Color color;
    string description;
} WorldMapRegionDescriptor;

typedef struct GlobalMapTile { //?
    char c;
    Color color;
} GlobalMapTile;
