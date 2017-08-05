#include "menu.h"

MenuEntry make_menu_entry(char *text, char key, int id, Color color) {
    MenuEntry menu_entry = malloc(sizeof(MenuEntry));
    menu_entry->text = text;
    menu_entry->key = key;
    menu_entry->id = id;
    menu_entry->color = color;
    return menu_entry;
}

void free_menu_entry(MenuEntry menu_entry) {
    free(menu_entry->text);
    free(menu_entry);
}
