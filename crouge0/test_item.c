#include "item.h"
#include <stdio.h>
#include <assert.h>

static const char *test_item_line = "wood sword:/:1:1:_cn_white:1:1:ItemWeaponCls:ItemNormalState:ItemWeaponSword:1:0:\n";
int item_fields_count = 12;

void test_dumps() {
    Item i = item_new('i', 0, 0);
    item_free(i);

    Items is = NULL;
    item_add(&is, item_new('x', 0, 0));
    i = items_get(is, 0, 0);
    assert(i != NULL);
    items_free(&is);

    extern char *ItemClassNames[];
    extern char *ItemStateNames[];
    extern char *ItemTypeNames[];
    printf("item cls from str: %s\n", ItemClassNames[ItemWeaponCls]);
    printf("item cls from str: %s\n", ItemClassNames[ItemHeadEquipCls]);
    assert(ItemWeaponCls == ItemClass_from_str("ItemWeaponCls"));
    assert(ItemWeaponSword == ItemType_from_str("ItemWeaponSword"));
    assert(ItemNormalState == ItemState_from_str("ItemNormalState"));
    printf("item type from str: %s\n", ItemTypeNames[ItemStrawHat]);
    printf("item state from str: %s\n", ItemStateNames[ItemBlessedState]);

    /* (de)serialize one item */
    printf("Load: \n");
    char *test = strdup(test_item_line);
    Strings s = parse_dsv_line(test, item_fields_count);
    Item item = item_deserialize(s);
    char *dsc = item_descript(item);
    printf("%s\n", dsc);
    free(dsc);

    char *dump = item_serialize(item);
    printf("serialized: %s\n", dump);
    assert(strcmp(dump, test) == 0);

    free(dump);
    item_free(item);
    free_dsv_strings(s);
    free(test);

    /* load/save list of items */
    Items items = items_load("./maps/map_1_1.items");
    items_save("/tmp/items", items);
    items_free(&items);


#include <libgen.h>
    char *full_mapname = strdup("../maps/map_1_1");
    char *base_path = strdup(dirname(full_mapname));
    free(full_mapname);
    printf("%s\n", base_path);
    free(base_path);
}

void test_clone() {
    Items items = items_load("./maps/you.items");
    Item a = g_list_nth_data(items, 0);
    Item b = item_clone(a);
    item_free(b);
    Items is = items_clone(items);

    GList *i = is;
    while (i) {
        Item itm = i->data;
        printf("->%s\n", itm->name);
        i = g_list_next(i);
    }

    items_free(&items);
    items_free(&is);
}

int main() {
    test_dumps();
    test_clone();
}
