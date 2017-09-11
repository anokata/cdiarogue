#include "charinfo.h"

int charinfo_draw(void* data) {
    UNUSED(data);
    clear();
    cc_printxy("@", cn_white, 0, 0);
    return 0;
}

int charinfo_key(void* data) {
    G g = data;
    char key = g->key;
    switch (key) {
        case 'q':
            ss_setstate(state, State_cursor);
            break;
    }
    return 0;
}
