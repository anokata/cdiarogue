#pragma once

#include "map.h"
#include "map_gen.h"
#include "tile_map.h"
#include "world_map.h"
#include "actor.h"
#include "anim.h"
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
    State_charinfo,
    State_takeoff,
    State_anim,
    NUM_STATES};

enum Events {
    Event_draw, 
    Event_key, 
    NUM_EVENTS};

struct G;

typedef void (*EventFunc)(struct G *g);

typedef struct GlobalEvent {
    EventFunc f;
    int probability;
} GlobalEvent;

bool roll_dice(int probability); /* probability 0..100 */

/* TODO extract location */
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
    GlobalEvent *events;
    int debug;
    char *location_path;
    char *savefile;
    int turns;
    unsigned int mode;
    Anim anim;
} *G;

#define G_STAT_EDIT 1

G new_g();
void free_g(G g);
void debuglog(G g, char *msg);
void add_actor(G g, Actor actor);
bool g_is_stat_edit(G g);

State state;

void proc_global_events(G g);
