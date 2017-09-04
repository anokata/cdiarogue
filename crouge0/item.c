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
