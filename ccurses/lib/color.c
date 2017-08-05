#include "color.h"

void cc_init_colors() {
	init_pair(_cn_red, COLOR_RED, BACKGROUND);
	init_pair(_cn_blue, COLOR_BLUE, BACKGROUND);
	init_pair(_cn_yellow, COLOR_YELLOW, BACKGROUND);
	init_pair(_cn_white, COLOR_WHITE, BACKGROUND);
	init_pair(_cn_black, COLOR_BLACK, BACKGROUND);
	init_pair(_cn_green, COLOR_GREEN, BACKGROUND);
	init_pair(_cw_white, COLOR_WHITE, COLOR_WHITE);
}

Color cc_get_color_by_id(int id) {
    int len = sizeof(cc_all_colors);
    for (int i = 0; i < len; ++i) {
        if (cc_all_colors[i].color == id) return cc_all_colors[i];
    }
    return cc_all_colors[0];
}
