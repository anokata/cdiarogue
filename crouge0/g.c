#include "g.h"

void _rand_msg(G g) {
    debuglog(g, "random msg");
}

GlobalEvent events[] = {
    {NULL, 50}, /* strange bug - first element has trash in f */
    {_rand_msg, 1},
};

void debuglog(G g, char *msg) {
    g->log_len++;
    g->log = g_list_append(g->log, g_strdup(msg));
}

G new_g() {
    G g = malloc(sizeof(struct G));

    g->events = events;
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
    g->view->display_left = 1;
    g->view->display_top = 1;

    g->log = NULL;
    g->log_len = 0;

    // make_player(
    g->player = make_actor('@', 0, 0);
    g->player->role = RolePlayer;
    g->player->color = cb_white;
    g->player->behavior = BehaviorStand;
    g->player->name = "you";
    g->player->stat_hp = 27;
    g->player->basestat_constitution = 7;
    add_actor(g, g->player);
    g->last_target = NULL;
    // init inventory
    Item potion = item_new('!', 1, 1);
    potion->type = ItemPotionOfCure;
    potion->cls = ItemPotionCls;
    item_add(&g->player->items, potion);
    potion = item_new('!', 1, 1);
    potion->type = ItemPotionOfCure;
    potion->cls = ItemPotionCls;
    item_add(&g->player->items, potion);

    Item sword = item_new('/', 0, 0);
    sword->cls = ItemWeaponCls;
    sword->type = ItemWeaponSword;
    item_add(&g->player->items, sword);

    Item helm = item_new(']', 0, 0);
    helm->cls = ItemHeadEquipCls;
    helm->type = ItemStrawHat;
    item_add(&g->player->items, helm);

    return g;
}

void gfree(gpointer data, gpointer user_data) {
    UNUSED(user_data);
    free(data);
}

void free_g(G g) {
    free_wmap(g->wmap);

    free_actors(g->gmap->actors);
    g_list_free(g->gmap->actors);
    g->gmap->actors = NULL;

    free_tile_map(g->gmap);
    free(g->view);
    g_list_foreach(g->log, gfree, NULL);
    g_list_free(g->log);
    //free(g->player);
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
