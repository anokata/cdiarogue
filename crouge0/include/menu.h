#pragma once
#ifndef __MENU__
#define __MENU__
#include "lib/ccurses.h"

// MenuEntry: text, key, color, func, id
typedef struct MenuEntry {
    char *text;
    char key;
    int id;
    Color color;
} *MenuEntry;

typedef struct Menu {
    MenuEntry *entries;
} *Menu;

MenuEntry make_menu_entry(char *text, char key, int id, Color color);
void free_menu_entry(MenuEntry menu_entry);
// make_menu
// add_menu_entry
// free_menu

#endif

