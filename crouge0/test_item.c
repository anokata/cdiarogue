#include "item.h"
#include <stdio.h>
#include <assert.h>

int main() {
    Item i = item_new('i', 0, 0);
    item_free(i);

    Items is = NULL;
    item_add(&is, item_new('x', 0, 0));
    i = items_get(is, 0, 0);
    assert(i != NULL);
    items_free(&is);

    extern char *ItemClassNames[];
    printf("item cls from str: %s\n", ItemClassNames[ItemWeaponCls]);
    printf("item cls from str: %s\n", ItemClassNames[ItemHeadEquipCls]);
    assert(ItemWeaponCls == ItemClass_from_str("ItemWeaponCls"));
}
