#pragma once

#include "map.h"
#include "map_gen.h"
#include "tile_map.h"
#include "world_map.h"

typedef struct G {
    WorldMap wmap;
    TileMap gmap;
    Point cursor;
    char key;
    Viewport *view;
} *G;

G new_g();
void free_g(G g);
