#pragma once

#include "map.h"
#include "map_gen.h"
#include "tile_map.h"
#include "world_map.h"
#include "actor.h"
#include <glib.h>

enum States {
    State_run, 
    State_end, 
    State_wmap, 
    State_cursor, 
    State_inventory, 
    State_drop,
    State_quaff,
    State_equip,
    NUM_STATES};

enum Events {
    Event_draw, 
    Event_key, 
    NUM_EVENTS};

typedef struct G {
    WorldMap wmap;
    TileMap gmap;
    Point cursor;
    char key;
    Viewport *view;
    GList *log;
    int log_len;
    Actor player;
    Actor last_target;
} *G;

G new_g();
void free_g(G g);
void debuglog(G g, char *msg);
void add_actor(G g, Actor actor);

State state;
