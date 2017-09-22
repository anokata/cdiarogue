#pragma once
#ifndef __MAP_GEN__
#define __MAP_GEN__

/* #define _GNU_SOURCE */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "lib/ccurses.h"
#include "util.h"
#include "map.h"
#include "tile_map.h"


/* Map make_map(int width, int height); */
/* void free_map(Map map); */
/* void print_map(TileMap map); */
int out_map(char *filename, int width, int height);

int fget_int_line(FILE *file);
char map_char_at(TileMap map, int x, int y);
void save_map(Map map, char *filename);
void save_tilemap(TileMap map, char *filename);
Map gen_map(int width, int height);
Map gen_map_rooms_split(int width, int height);
void print_map(Map map);
void free_map(Map map);
Map tilemap_convert2map(TileMap tmap);
TileMap map_convert2tilemap(Map map);


#endif
