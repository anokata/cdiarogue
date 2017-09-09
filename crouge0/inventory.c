#include "inventory.h"

extern State state;

int inventory_key(void* data) {
    G g = data;
    char key = g->key;

    switch (key) {
        case 'i':
            ss_setstate(state, State_cursor);
            break;
        case '?':
            break;
        default:
            break;
    }
    /* get item index by key, if correct, see state and call function on item */
    int selected_item_index = key - 'a';
    Item item = inventory_get_by_idx(g->player->items, selected_item_index);
    if (!item) return 0;
    inventory_action(item, g);
    return 0;
}

/* need test */
void drop_item(Actor actor, TileMap map, Item item) {
    /* set item coords */
    item->x = actor->x;
    item->y = actor->y;
    /* place to map */
    tmap_add_item(map, item);
    /* remove from inventory */
    item_remove(&actor->items, item);
}

void inventory_action(Item item, G g) {
    char *description = NULL;
    switch (ss_get_state(state)) {
        case State_drop:
            description = item_descript(item);
            debuglog(g, "you drop the");
            debuglog(g, description);
            free(description);
            /* actuall drop item from map */
            drop_item(g->player, g->gmap, item);

            ss_setstate(state, State_cursor);
            break;
        case State_inventory:
            description = item_descript(item);
            debuglog(g, "you choose");
            debuglog(g, description);
            free(description);
            ss_setstate(state, State_cursor);
            break;
    }
}

int inventory_draw(void* data) {
    G g = data;
    UNUSED(g);

    char buf[BUFSIZE];
    clear();
    int items_count = g_list_length(g->player->items);
    snprintf(buf, BUFSIZE, "Inventory %d items:", items_count);
    cc_printxy(buf, cn_white, 0, 0);
    GList *it = g->player->items;
    int y = 0;
    while (it) {
        Item item = it->data;
        // wtf with % sign?
        char *description = item_descript(item);
        snprintf(buf, BUFSIZE, "%c) %c - %s", (y + 'a'), item->c, description);
        free(description);
        cc_printxy(buf, cn_white, 2, ++y);
        it = g_list_next(it);
    }

    return 0;
}

Item inventory_get_by_idx(Items items, int index) {
    int items_count = g_list_length(items);

    if ((index < items_count) && (index >= 0)) {
        return g_list_nth_data(items, index);
    }
    return NULL;
}
