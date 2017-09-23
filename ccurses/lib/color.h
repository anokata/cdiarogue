#pragma once
#ifndef __COLOR__
#define __COLOR__
#include <ncurses.h>
#include <string.h>

static const int BACKGROUND = -1; // COLOR_BLACK

typedef struct Color {
    int color; // enum ColorID
    int attr;
} Color;

typedef int color_index;

#define MAKE_STRING(STR) #STR,
#define MAKE_ENUM(X) X,

#define FOREACH_COLOR(X) \
    X(_cn_no) \
    X(_cn_red) \
    X(_cb_red) \
    X(_cw_red) \
    X(_cn_green) \
    X(_cb_green) \
    X(_cw_green) \
    X(_cn_black) \
    X(_cb_black) \
    X(_cw_black) \
    X(_cn_white) \
    X(_cb_white) \
    X(_cw_white) \
    X(_cc_white) \
    X(_cn_yellow) \
    X(_cb_yellow) \
    X(_cw_yellow) \
    X(_cn_blue) \
    X(_cb_blue) \
    X(_cw_blue) \

enum ColorID {
    FOREACH_COLOR(MAKE_ENUM)
};


static const struct Color cn_red = {_cn_red, A_NORMAL};
static const struct Color cb_red = {_cn_red, A_BOLD};
static const struct Color cd_red = {_cn_red, A_DIM};
static const struct Color cn_blue = {_cn_blue, A_NORMAL};
static const struct Color cb_blue = {_cn_blue, A_BOLD};
static const struct Color cd_blue = {_cn_blue, A_DIM};
static const struct Color cn_yellow = {_cn_yellow, A_NORMAL};
static const struct Color cb_yellow = {_cn_yellow, A_BOLD};
static const struct Color cd_yellow = {_cn_yellow, A_DIM};
static const struct Color cn_white = {_cn_white, A_NORMAL};
static const struct Color cb_white = {_cn_white, A_BOLD};
static const struct Color cd_white = {_cn_white, A_DIM};
static const struct Color cn_green = {_cn_green, A_NORMAL};
static const struct Color cb_green = {_cn_green, A_BOLD};
static const struct Color cd_green = {_cn_green, A_DIM};
static const struct Color cw_white = {_cw_white, A_NORMAL};

static const Color cc_all_colors[] = {
    {_cn_red, A_NORMAL},
    {_cb_red, A_BOLD},
    {_cw_red, A_DIM},
    {_cn_blue, A_NORMAL},
    {_cb_blue, A_BOLD},
    {_cw_blue, A_DIM},
    {_cn_yellow, A_NORMAL},
    {_cb_yellow, A_BOLD},
    {_cw_yellow, A_DIM},
    {_cn_white, A_NORMAL},
    {_cb_white, A_BOLD},
    {_cw_white, A_DIM},
    {_cc_white, A_NORMAL},
    {_cn_green, A_NORMAL},
    {_cb_green, A_BOLD},
    {_cw_green, A_DIM},
    {_cn_black, A_NORMAL},
    {_cb_black, A_BOLD},
    {_cw_black, A_DIM},
};

void cc_init_colors();
Color cc_get_color_by_id(int id);
Color cc_color_from_str(char *str);

#endif
