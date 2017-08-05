#pragma once
#ifndef __WORLD_MAP__
#define __WORLD_MAP__

#include "map.h"
#include "map_gen.h"

WorldMap load_wmap();
void free_wmap(WorldMap wmap);
void print_wmap(WorldMap wmap);

#endif
