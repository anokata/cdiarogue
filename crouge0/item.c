#include "item.h"

IMPLEMENT_ENUM(ItemClass, ITEMS_CLS)
IMPLEMENT_ENUM(ItemState, ITEMS_STATE)
IMPLEMENT_ENUM(ItemType, ITEMS_TYPES)


extern char *ColorNames[];
const char item_file_header[] = "name:char:x:y:color:value:class:state:type:\n";
const char item_file_type[] = "# vi: filetype=sh\n";
const char item_dump_format[] = "%s:%c:%d:%d:%s:%d:%s:%s:%s:\n";

char *item_types[] = {
    "potion of cure wounds",
    "wood sword",
    "simple straw hat",
};

char *item_states[] = {
    " ",
    "blessed",
    "cursed",
};

Item item_new(char c, int x, int y) {
    Item item = malloc(sizeof(struct Item));
    item->c = c;
    item->x = x;
    item->y = y;
    item->color = cn_white;
    item->name = NULL;
    item->state = ItemNormalState;
    item->type = ItemWeaponSword;
    item->cls = ItemNoCls;
    item->value = 1;
    return item;
}

void item_free(Item item) {
    if (item->name) free(item->name);
    free(item);
}

void item_add(Items *items, Item item) {
    *items = g_list_append(*items, item);
}

void item_remove(Items *items, Item item) {
    *items = g_list_remove(*items, item);
    free(item);
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

char *item_descript(Item item) {
    if (!item) return "";
    char *buf = malloc(BUFSIZE); // FIXME
    // check for null in type, state, ...
    snprintf(buf, BUFSIZE, "%s %s", 
            item_states[item->state],
            item_types[item->type]
    );
    
    return buf;
}

int item_value(Item item) {
    if (!item) return 0;
    return item->value;
}

Item item_deserialize(Strings str) {
    char c = str[1][0];
    int x = atoi(str[2]);
    int y = atoi(str[3]);
    Item item = item_new(c, x, y);
    item->name = strdup(str[0]);
    item->color = cc_color_from_str(str[4]);
    item->value = atoi(str[5]);
    item->cls = ItemClass_from_str(str[6]);
    item->state = ItemState_from_str(str[7]);
    item->type = ItemType_from_str(str[8]);
    return item;
}

char *item_serialize(Item item) {
    char *buf = malloc(BUFSIZE);
    snprintf(buf, BUFSIZE, item_dump_format,
            item->name,
            item->c,
            item->x,
            item->y,
            ColorNames[item->color.color],
            item->value,
            ItemClassNames[item->cls],
            ItemStateNames[item->state],
            ItemTypeNames[item->type]
    );
    return buf;
}
