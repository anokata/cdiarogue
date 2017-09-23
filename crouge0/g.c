#include "g.h"

void _rand_msg(G g) {
    debuglog(g, "random msg");
}

void player_regen(G g) {
    actor_heal(g->player, actor_stat_regen(g->player));
}

GlobalEvent events[] = {
    {NULL, 50}, /* strange bug - first element has trash in f */
    {_rand_msg, 1},
    {player_regen, 80},
};

void debuglog(G g, char *msg) {
    g->log_len++;
    g->log = g_list_append(g->log, g_strdup(msg));
}

G new_g() {
    G g = malloc(sizeof(struct G));
    bzero(g, sizeof(struct G));

    g->events = events;
    g->wmap = load_wmap();
    g->gmap = load_global_tmap();
    g->cursor.x = 0;
    g->cursor.y = 0;

    g->view = malloc(sizeof(struct Viewport));
    // TODO move to loaded player 
    g->view->cx = 0;
    g->view->cy = 0;
#ifndef WORK
    g->view->width = 40;
    g->view->height = 40;
#else
    g->view->width = 8;
    g->view->height = 8;
#endif
    g->view->display_left = 1;
    g->view->display_top = 1;

    g->log = NULL;
    g->log_len = 0;

    g->last_target = NULL;

    return g;
}

void gfree(gpointer data, gpointer user_data) {
    UNUSED(user_data);
    free(data);
}

void free_g(G g) {
    free_wmap(g->wmap);

    free_actors(&g->gmap->actors);
    g->gmap->actors = NULL;

    free_tile_map(g->gmap);
    free(g->view);
    g_list_foreach(g->log, gfree, NULL);
    g_list_free(g->log);
    free(g);
}

void add_actor(G g, Actor actor) {
    g->gmap->actors = g_list_append(g->gmap->actors, actor);
}

bool roll_dice(int probability) {
    return (rand() % 101) <= probability;
} 

void proc_global_events(G g) {
    int events_count = sizeof(events) / sizeof(events[0]);
    for (int i = 1; i < events_count; i++) {
        GlobalEvent event = g->events[i];
        if (roll_dice(event.probability)) {
            event.f(g);
        }
    }
}
