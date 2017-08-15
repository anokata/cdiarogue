#pragma once
#ifndef __MAP_GEN__
#define __MAP_GEN__

/* #define _GNU_SOURCE */
#define DEBUG

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "lib/ccurses.h"
#include "util.h"
#include "map.h"


Map make_map(int width, int heigth);
Map load_map(string filename);
void free_map(Map map);

void print_map(Map map);
int out_map(char *filename, int width, int heigth);

int fget_int_line(FILE *file);
char map_char_at(TileMap map, int x, int y);


#endif
