#pragma once

#include <glib.h>
#include "lib/ccurses.h"
#include "actor.h"
#include "tile_map.h"
#include "util.h"

void draw_actor(Actor actor, int x, int y);
void draw_actors(GList *actors, Viewport *v);
bool in_viewport(Viewport *v, int x, int y);
void draw_actor_self(Actor actor, Viewport *v);
