#pragma once
#include <stdbool.h>
#include "map.h"
#include "actor.h"
#include "tile_map.h"

typedef Point (*ActorMoveFunc)(Actor actor, TileMap map);
/* Behavior */
Point actor_step_at_directed(Actor actor, TileMap map);
Point actor_move_rand(Actor actor, TileMap map);
Point actor_not_move(Actor actor, TileMap map);
Point actor_simple_attack(Actor actor, TileMap map);

Point _actor_move_direct(Actor actor, TileMap map);
void _actor_choose_direct_point_rand(Actor actor, TileMap map);

bool actor_move_hv(Actor actor, TileMap map, int h, int v);
void player_move(Actor player, Viewport *view);
Point actor_get_move_point(Actor actor, TileMap map);
