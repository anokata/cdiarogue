#pragma once
#include <stdlib.h>
#include <string.h>

typedef u_int8_t lightcell;
typedef lightcell *LightMap;

LightMap lightmap_new(int width, int height);
void lightmap_free(LightMap lm);
