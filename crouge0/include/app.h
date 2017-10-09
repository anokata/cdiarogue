#pragma once
#ifndef __APP__
#define __APP__


#include "lib/ccurses.h"
#include "messages.h"
#include "menu.h"
#include "map.h"
#include "map_gen.h"
#include "tile_map.h"
#include "world_map.h"
#include "actor.h"
#include "behavior.h"
#include "g.h"
#include "render.h"
#include "collide.h"
#include "item.h"
#include "inventory.h"
#include "charinfo.h"
#include "lightcalc.h"

void debug_draw(G g);
void start(char *arg1);

#endif
