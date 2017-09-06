#include "inventory.h"

extern State state;

int inventory_key(void* data) {
    G g = data;
    char key = g->key;

    switch (key) {
        case 'i':
            ss_setstate(state, State_cursor);
            break;
        case '?':
            break;
        default:
            break;
    }
    return 0;
}

int inventory_draw(void* data) {
    G g = data;
    UNUSED(g);

    clear();
    cc_printxy("Inventory:", cn_white, 0, 0);

    return 0;
}
