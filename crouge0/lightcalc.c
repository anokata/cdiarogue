#include "lightcalc.h"
#include <math.h>

void map_lightning(TileMap map, int cx, int cy) {
    lightmap_clear(map->lightmap, map->width, map->height);
    const float angle_step = 2 * M_PI / 360;
    const float r_step = 0.5;
    float radius = 8.0;
    int x, y;
    for (float angle = 0.0; angle < 2 * M_PI; angle += angle_step) {
        for (float r = 1; r < radius; r += r_step) {
            x = cosf(angle) * r + cx;
            y = sinf(angle) * r + cy;
            /* printf("%d %d\n", x, y); */
            map->lightmap[ y * map->width + x] = 1;
            if (!tile_at(map, x, y)->passable) {
                /* break this ray at not passable tile lighted */
                break;
            }
        }
    }
}
