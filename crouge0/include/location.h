#pragma once

#include "g.h"
#include "tile_map.h"
#include "item.h"

G g_init(char *arg1);

void save_location(TileMap map);
void load(G g);
void save(G g);
void save_savefile(G g);
void load_savefile(G g);

bool is_special_location(char *location_path);
void change_map(G g, char *location_filename);
void enter_location(G g);
