#include "g.h"

void debuglog(G g, char *msg) {
    g->log_len++;
    g->log = g_list_append(g->log, g_strdup(msg));
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
#ifndef WORK
    g->view->width = 40;
    g->view->height = 40;
#else
    g->view->width = 8;
    g->view->height = 8;
#endif
    g->view->display_left = 8;
    g->view->display_top = 8;

    g->log = NULL;
    g->log_len = 0;

    g->actors = NULL;

    // make_player(
    g->player = make_actor('@', 0, 0);
    g->player->role = RolePlayer;
    g->player->color = cb_white;
    g->player->behavior = BehaviorStand;
    g->player->name = "you";
    add_actor(g, g->player);
    g->last_target = NULL;

    return g;
}

void gfree(gpointer data, gpointer user_data) {
    UNUSED(user_data);
    free(data);
}

void free_g(G g) {
    free_wmap(g->wmap);
    free_tile_map(g->gmap);
    free(g->view);
    g_list_foreach(g->log, gfree, NULL);
    g_list_free(g->log);
    free_actors(g->actors);
    g_list_free(g->actors);
    //free(g->player);
    free(g);
}

void add_actor(G g, Actor actor) {
    g->actors = g_list_append(g->actors, actor);
}
