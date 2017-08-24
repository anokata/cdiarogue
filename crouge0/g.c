#include "g.h"

void debuglog(G g, char *msg) {
    g->log_len++;
    g->log = g_list_append(g->log, msg);
}

G new_g() {
    G g = malloc(sizeof(struct G));

    g->wmap = load_wmap();
    g->gmap = load_global_tmap();
    g->cursor.x = 0;
    g->cursor.y = 0;

    g->view = malloc(sizeof(Viewport));
    g->view->cx = 0;
    g->view->cy = 0;
    /* g->view->width = 30; */
    /* g->view->height = 30; */
    g->view->width = 8;
    g->view->height = 8;
    g->view->display_left = 8;
    g->view->display_top = 8;

    g->log = NULL;
    g->log_len = 0;

    g->actors = NULL;

    return g;
}

void free_g(G g) {
    free_wmap(g->wmap);
    free_tile_map(g->gmap);
    free(g->view);
    g_list_free(g->log);
    free_actors(g->actors);
    g_list_free(g->actors);
    free(g);
}

void add_actor(G g, Actor actor) {
    g->actors = g_list_append(g->actors, actor);
}
