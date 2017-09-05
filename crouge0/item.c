#include "item.h"

Item item_new(char c, int x, int y) {
    Item item = malloc(sizeof(struct Item));
    item->c = c;
    item->x = x;
    item->y = y;
    item->color = cn_white;
    item->name = NULL;
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

Item items_get(Items items, int x, int y) {
    GList *it = items;
    while (it) {
        Item item = it->data;
        if ((item->x == x) && (item->y == y)) {
            return item;
        }
        it = g_list_next(it);
    }
    return NULL;
}
