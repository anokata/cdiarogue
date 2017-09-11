#pragma once
#include <stdlib.h>
#include <glib.h>
#include "util.h"

typedef enum ItemClass {
    ItemNoCls,
    ItemPotionCls,
    ItemWeaponCls,
    ItemBodyEquipCls,
    ItemFootEquipCls,
    ItemLegsEquipCls,
    ItemHeadEquipCls,
    ItemFingerEquipCls,
    ItemFoodCls,
} ItemClass;

typedef enum ItemState {
    ItemNormalState,
    ItemBlessedState,
    ItemCursedState,
} ItemState;

typedef enum ItemType {
    ItemPotionOfCure,
    ItemWeaponSword,
    ItemStrawHat,
} ItemType;

typedef struct Item {
    char c;
    int x;
    int y;
    Color color;
    int value;
    char *name;
    ItemClass cls;
    ItemState state;
    ItemType type;
} *Item;

typedef GList *Items;

Item item_new(char c, int x, int y);
void item_free(Item item);

void items_free(Items *items);
void item_add(Items *items, Item item);
void item_remove(Items *items, Item item);
CharPoint item_as_charpoint_cast(Item item);
Item items_get(Items items, int x, int y);

char *item_descript(Item item);
int item_value(Item item);
