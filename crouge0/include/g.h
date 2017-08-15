#pragma once

#include "map.h"
#include "map_gen.h"
#include "tile_map.h"
#include "world_map.h"
#include <glib.h>

typedef struct G {
    WorldMap wmap;
    TileMap gmap;
    Point cursor;
    char key;
    Viewport *view;
    GList *log;
    int log_len;
} *G;

G new_g();
void free_g(G g);
void debuglog(G g, char *msg);
