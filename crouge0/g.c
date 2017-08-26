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
    g->view->width = 40;
    g->view->height = 40;
    /* g->view->width = 8; */
    /* g->view->height = 8; */
    g->view->display_left = 8;
    g->view->display_top = 8;

    g->log = NULL;
    g->log_len = 0;

    g->actors = NULL;

    g->player = make_actor('@', 0, 0);
    g->player->role = RolePlayer;
    g->player->color = cb_white;
    g->player->behavior = BehaviorStand;
    add_actor(g, g->player);

    return g;
}

void free_g(G g) {
    free_wmap(g->wmap);
    free_tile_map(g->gmap);
    free(g->view);
    g_list_free(g->log);
    free_actors(g->actors);
    g_list_free(g->actors);
    //free(g->player);
    free(g);
}

void add_actor(G g, Actor actor) {
    g->actors = g_list_append(g->actors, actor);
}
