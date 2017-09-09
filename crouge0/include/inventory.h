#pragma once
#include "item.h"
#include "g.h"

int inventory_key(void* data);
int inventory_draw(void* data);

Item inventory_get_by_idx(Items items, int index);
void inventory_action(Item item, G g);
