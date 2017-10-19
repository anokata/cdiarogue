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
#include "anim.h"

void debug_draw(G g);
void start(char *arg1);
G g_init(char *arg1);
void load(G g);
void g_post_init(G g);

#endif
