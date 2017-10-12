#pragma once

#include <glib.h>
#include "lib/ccurses.h"
#include "actor.h"
#include "tile_map.h"
#include "util.h"
#include "g.h"

void draw_charpoint(CharPoint cp, int x, int y);
void draw_charpoints(GList *elements, G g);
bool in_viewport(Viewport *v, int x, int y);
void draw_actor_self(Actor actor, Viewport *v);
