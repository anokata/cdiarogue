#include "item.h"

IMPLEMENT_ENUM(ItemClass, ITEMS_CLS)
IMPLEMENT_ENUM(ItemState, ITEMS_STATE)
IMPLEMENT_ENUM(ItemType, ITEMS_TYPES)


extern char *ColorNames[];

char *item_types[] = {
    "potion of cure wounds",
    "wood sword",
    "simple straw hat",
    "paw of day darkness knuckles",
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

Item item_clone(Item item) {
    Item clone = item_new(item->c, item->x , item->y);
    memcpy(clone, item, sizeof(struct Item));
    clone->name = strdup(item->name);
    return clone;
}

void item_add(Items *items, Item item) {
    *items = g_list_append(*items, item);
}

void item_detach(Items *items, Item item) {
    *items = g_list_remove(*items, item);
}

void item_remove(Items *items, Item item) {
    *items = g_list_remove(*items, item);
    item_free(item);
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
    return (Item) charpoint_at(items, x, y);
    /* GList *it = items; */
    /* while (it) { */
    /*     Item item = it->data; */
    /*     if ((item->x == x) && (item->y == y)) { */
    /*         return item; */
    /*     } */
    /*     it = g_list_next(it); */
    /* } */
    /* return NULL; */
}

char *item_descript(Item item) {
    if (!item) return "";
    char *buf = malloc(BUFSIZE); // FIXME
    // check for null in type, state, ...
    switch (item->cls) {
        case ItemWeaponCls:
            snprintf(buf, BUFSIZE, "%s %s dmg(%d-%d)", 
                    item_states[item->state],
                    item_types[item->type],
                    item->value - item->dispersion,
                    item->value + item->dispersion
            );
            break;
        default:
            snprintf(buf, BUFSIZE, "%s %s", 
                    item_states[item->state],
                    item_types[item->type]
            );
            break;
    }
    
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
    item->dispersion = atoi(str[6]);
    item->cls = ItemClass_from_str(str[7]);
    item->state = ItemState_from_str(str[8]);
    item->type = ItemType_from_str(str[9]);
    item->count = atoi(str[10]);
    item->mode = atoi(str[11]);
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
            item->dispersion,
            ItemClassNames[item->cls],
            ItemStateNames[item->state],
            ItemTypeNames[item->type],
            item->count,
            item->mode
    );
    return buf;
}

Items items_load(char *filename) {
    Items items = NULL; 
    filename = strdup(filename);
    StringTable st = parse_dsv_file(filename);
    StringTable it = &st[1]; // first line is header
    do {
        Item item = item_deserialize(*it);
        item_add(&items, item); 
        DEBUG_PRINT("load item: %s\n", item->name);
    } while (*++it);

    free_dsv_table(st);
    free(filename);
    return items;
}

void items_save(char *filename, Items items) {
    GList *it = items;
    FILE *out = fopen(filename, "w+");
    fwrite(item_file_header, strlen(item_file_header), 1, out);
    fwrite(item_file_type, strlen(item_file_type), 1, out);

    while (it) {
        Item item = it->data;
        char *line = item_serialize(item);
        fwrite(line, strlen(line), 1, out);
        free(line);
        it = g_list_next(it);
    }

    fclose(out);
}

bool item_is_equiped(Item item) {
    return (item->mode && ITEM_MODE_EQUIPED);
}

void item_set_equiped(Item item) {
    item->mode |= ITEM_MODE_EQUIPED;
}

void item_unset_equiped(Item item) {
    item->mode ^= ITEM_MODE_EQUIPED;
}
