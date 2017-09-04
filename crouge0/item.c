#include "item.h"

Item item_new(char c, int x, int y) {
    Item item = malloc(sizeof(struct Item));
    item->c = c;
    item->x = x;
    item->y = y;
    return item;
}

void item_free(Item item) {
    free(item);
}

void item_add(Items *items, Item item) {
    *items = g_list_append(*items, item);
}

void items_free(Items *items) {
    if (!*items) return;
    GList *it = *items;
    while (it) {
        item_free((Item) it->data);
        it = g_list_next(it);
    }
    g_list_free(*items);
    *items = NULL;
}

CharPoint item_as_charpoint_cast(Item item) {
    return (struct CharPoint *) item;
}
