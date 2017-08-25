#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include <glib.h>
#include "map.h"
#include "tile_map.h"

typedef enum Behavior { BehaviorRandom=0, BehaviorSimpleDirect } Behavior;
typedef enum Role { RolePlayer, RoleMonster, RoleNPC } Role;

typedef struct Actor {
    char c;
    int x;
    int y;
    Behavior behavior;
    Point directed;
    Color color;
    int stat_attack;
    Role role;
} *Actor;

typedef void (*ActorMoveFunc)(Actor actor, TileMap map);

Actor make_actor(char c, int x, int y);
void free_actors(GList *actors);
bool actor_isat_directed_place(Actor actor);
void actor_step_at_directed(Actor actor, TileMap map);
void actor_move_rand(Actor actor, TileMap map);
void actor_move_direct(Actor actor, TileMap map);
void actor_move(Actor actor, TileMap map);
void actor_choose_direct_point_rand(Actor actor, TileMap map);

bool actor_move_hv(Actor actor, TileMap map, int h, int v);
void player_move(Actor player, Viewport *view);
