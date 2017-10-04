#include "lightmap.h"

LightMap lightmap_new(int width, int height) {
    size_t size = width * height * sizeof(lightcell);
    LightMap lm = malloc(size);
    memset(lm, 0, size);
    return lm;
}

void lightmap_free(LightMap lm) {
    if (lm) free(lm);
}

void lightmap_clear(LightMap lm, int width, int height) {
    size_t size = width * height * sizeof(lightcell);
    memset(lm, 0, size);
}
