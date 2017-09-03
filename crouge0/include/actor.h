#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include <glib.h>
#include "map.h"
#include "tile_map.h"

typedef enum Behavior { 
    BehaviorRandom=0, 
    BehaviorSimpleDirect, 
    BehaviorStand, 
    BehaviorSimpleAttacker 
} Behavior;

typedef enum Role { 
    RolePlayer, 
    RoleMonster, 
    RoleNPC,
    RoleLength
} Role;

typedef enum Status {
    StatusLive,
    StatusDead,
    StatusSleep
} Status;

typedef struct Actor {
    char c;
    int x;
    int y;
    Behavior behavior;
    Point directed;
    Color color;
    Status status;
    int stat_attack;
    int stat_hp;
    char *name;
    Role role;
} *Actor;

typedef Point (*ActorMoveFunc)(Actor actor, TileMap map);

Actor make_actor(char c, int x, int y);
void free_actors(GList *actors);
void actor_free(Actor actor);
bool _actor_isat_directed_place(Actor actor);
int _actor_direct_diffx(Actor actor);
int _actor_direct_diffy(Actor actor);
Point actor_step_at_directed(Actor actor, TileMap map);
Point actor_move_rand(Actor actor, TileMap map);
Point _actor_move_direct(Actor actor, TileMap map);
Point actor_not_move(Actor actor, TileMap map);
void _actor_choose_direct_point_rand(Actor actor, TileMap map);

bool actor_move_hv(Actor actor, TileMap map, int h, int v);
void player_move(Actor player, Viewport *view);
Point actor_get_move_point(Actor actor, TileMap map);
