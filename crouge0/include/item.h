#pragma once
#include <stdlib.h>

typedef struct Item {
    char c;
    int x;
    int y;
} *Item;

Item item_new(char c, int x, int y);
void item_free(Item item);
