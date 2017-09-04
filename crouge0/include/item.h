#pragma once
#include <stdlib.h>
#include <glib.h>

typedef struct Item {
    char c;
    int x;
    int y;
} *Item;

typedef GList *Items;

Item item_new(char c, int x, int y);
void item_free(Item item);

void items_free(Items *items);
void item_add(Items *items, Item item);
