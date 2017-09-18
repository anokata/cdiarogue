#pragma once
#include <stdlib.h>
#include <glib.h>
#include "util.h"

#define DECLARE_ENUM(NAME, ELEMS) typedef enum NAME {\
    ELEMS(MAKE_ENUM)\
} NAME;\
NAME NAME##_from_str(char *str);

#define IMPLEMENT_ENUM(NAME, ELEMS) \
char *NAME##Names[] = {\
    ELEMS(MAKE_STRING)\
    NULL\
};\
NAME NAME##_from_str(char *str) {\
    ENUM_FROMSTR(NAME)\
}\

#define ITEMS_CLS(X) \
    X(ItemNoCls) \
    X(ItemPotionCls) \
    X(ItemWeaponCls) \
    X(ItemBodyEquipCls) \
    X(ItemFootEquipCls) \
    X(ItemLegsEquipCls) \
    X(ItemHeadEquipCls) \
    X(ItemFingerEquipCls) \
    X(ItemFoodCls) \

DECLARE_ENUM(ItemClass, ITEMS_CLS)

#define ITEMS_STATE(X) \
    X(ItemNormalState) \
    X(ItemBlessedState) \
    X(ItemCursedState) \

DECLARE_ENUM(ItemState, ITEMS_STATE)

#define ITEMS_TYPES(X) \
    X(ItemPotionOfCure) \
    X(ItemWeaponSword) \
    X(ItemStrawHat) \

DECLARE_ENUM(ItemType, ITEMS_TYPES)

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
