#pragma once
#include <glib.h>
#include "actor.h"
#include "events.h"
#include "g.h"

bool collisions_player_move(Actor player, int dx, int dy, G g);
Actor collision_get_actor(Actor actor, int dx, int dy, GList *actors);
void collision_effect(Actor actor, Actor subject, G g);
ActionFunc _get_action(Actor actor, Actor subject, G g);

