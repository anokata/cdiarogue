#include "g.h"

G new_g() {
    G g = malloc(sizeof(struct G));

    g->wmap = load_wmap();
    g->gmap = load_global_tmap();
    g->cursor.x = 0;
    g->cursor.y = 0;

    g->view = malloc(sizeof(Viewport));
    g->view->cx = 0;
    g->view->cy = 0;
    g->view->width = 6;
    g->view->heigth = 6;

    return g;
}

void free_g(G g) {
    free_wmap(g->wmap);
    free_tile_map(g->gmap);
    free(g->view);
    free(g);
}
