#include "color.h"

char *ColorNames[] = {
    FOREACH_COLOR(MAKE_STRING)
};

Color cc_color_from_str(char *str) {
    char **it = ColorNames;
    enum ColorID val = 0;
    while (*it) {
        if (!strcmp(*it, str)) return cc_get_color_by_id(val);
        val++;
        it++;
    }
    return cc_get_color_by_id(_cn_white);
}

void cc_init_colors() {
	init_pair(_cn_red, COLOR_RED, BACKGROUND);
	init_pair(_cb_red, COLOR_RED, BACKGROUND);
	init_pair(_cw_red, COLOR_RED, BACKGROUND);
	init_pair(_cn_blue, COLOR_BLUE, BACKGROUND);
	init_pair(_cb_blue, COLOR_BLUE, BACKGROUND);
	init_pair(_cw_blue, COLOR_BLUE, BACKGROUND);
	init_pair(_cn_yellow, COLOR_YELLOW, BACKGROUND);
	init_pair(_cb_yellow, COLOR_YELLOW, BACKGROUND);
	init_pair(_cw_yellow, COLOR_YELLOW, BACKGROUND);
	init_pair(_cn_black, COLOR_BLACK, BACKGROUND);
	init_pair(_cw_black, COLOR_BLACK, BACKGROUND);
	init_pair(_cb_black, COLOR_BLACK, BACKGROUND);
	init_pair(_cn_green, COLOR_GREEN, BACKGROUND);
	init_pair(_cw_green, COLOR_GREEN, BACKGROUND);
	init_pair(_cb_green, COLOR_GREEN, BACKGROUND);
	init_pair(_cc_white, COLOR_WHITE, COLOR_WHITE);
	init_pair(_cn_white, COLOR_WHITE, BACKGROUND);
	init_pair(_cb_white, COLOR_WHITE, BACKGROUND);
	init_pair(_cw_white, COLOR_WHITE, BACKGROUND);
}

Color cc_get_color_by_id(int id) {
    int len = sizeof(cc_all_colors);
    for (int i = 0; i < len; ++i) {
        if (cc_all_colors[i].color == id) return cc_all_colors[i];
    }
    return cc_all_colors[0];
}
