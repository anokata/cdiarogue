#pragma once
#include <stdlib.h>
#include <glib.h>
#include "util.h"
#include "config_parser.h"

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
    X(ItemPoDDKnuckles) \

DECLARE_ENUM(ItemType, ITEMS_TYPES)

typedef struct Item {
    char c;
    int x;
    int y;
    Color color;
    int value;
    int dispersion;
    char *name;
    ItemClass cls;
    ItemState state;
    ItemType type;
    int count;
    u_int32_t mode; /* bit field (equiped,..) */
} *Item;

static const char item_file_header[] = "name:char:x:y:color:value:dispersion:class:state:type:count:mode:\n";
static const char item_file_type[] = "# vi: filetype=sh\n";
static const char item_dump_format[] = "%s:%c:%d:%d:%s:%d:%d:%s:%s:%s:%d:%d:\n";

#define ITEM_MODE_EQUIPED 1

typedef GList *Items;

Item item_new(char c, int x, int y);
void item_free(Item item);
Item item_clone(Item item);

void items_free(Items *items);
void item_add(Items *items, Item item);
void item_remove(Items *items, Item item);
void item_detach(Items *items, Item item);
CharPoint item_as_charpoint_cast(Item item);
Item items_get(Items items, int x, int y);
bool item_is_equiped(Item item);
void item_set_equiped(Item item);

char *item_descript(Item item);
int item_value(Item item);

Item item_deserialize(Strings str);
char *item_serialize(Item item);
Items items_load(char *filename);
void items_save(char *filename, Items items);
