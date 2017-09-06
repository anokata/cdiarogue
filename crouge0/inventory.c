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
    return 0;
}

int inventory_draw(void* data) {
    G g = data;
    UNUSED(g);

    clear();
    cc_printxy("Inventory:", cn_white, 0, 0);
    char buf[30];
    GList *it = g->player->items;
    int y = 0;
    while (it) {
        Item item = it->data;
        // wtf with % sign?
        snprintf(buf, 29, "%c) %c - ", (y + 'a'), item->c);
    debuglog(g, buf);
        cc_printxy(buf, cn_white, 2, ++y);
        it = g_list_next(it);
    }

    return 0;
}
