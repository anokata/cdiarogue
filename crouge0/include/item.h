#pragma once
#include <stdlib.h>
#include <glib.h>
#include "util.h"

typedef struct Item {
    char c;
    int x;
    int y;
    Color color;
} *Item;

typedef GList *Items;

Item item_new(char c, int x, int y);
void item_free(Item item);

void items_free(Items *items);
void item_add(Items *items, Item item);
CharPoint item_as_charpoint_cast(Item item);
