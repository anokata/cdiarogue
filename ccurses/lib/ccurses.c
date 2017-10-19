#include "ccurses.h"

void cc_printi(uint32_t value, struct Color color) {
    char str[20];
    snprintf(str, 20, "%d", value);
    cc_print(str, color);
}

void cc_print(char *str, struct Color color) {
	attron(color.attr);
    attron(COLOR_PAIR(color.color));
	printw(str);
	attroff(color.attr);
    attroff(COLOR_PAIR(color.color));
}

void cc_printxy(char *str, struct Color color, int x, int y) {
    move(y, x);
	attron(color.attr);
    attron(COLOR_PAIR(color.color));
	printw(str);
	attroff(color.attr);
    attroff(COLOR_PAIR(color.color));
}

void cc_put(char ch, struct Color color) {
	attron(color.attr);
    attron(COLOR_PAIR(color.color));
	addch(ch);
	attroff(color.attr);
    attroff(COLOR_PAIR(color.color));
}

void cc_putxy(char ch, struct Color color, int x, int y) {
    /* move(y, x); */
	attron(color.attr);
    attron(COLOR_PAIR(color.color));
	mvaddch(y, x, ch);
	attroff(color.attr);
    attroff(COLOR_PAIR(color.color));
}

void cc_refresh() {
    refresh();
}

void curses_init() {
	initscr();
    use_default_colors(); /* fror transparend -1 as back*/
	start_color();
	cc_init_colors();
	refresh();
    width = getmaxx(stdscr);
    heigth = getmaxy(stdscr);
    curs_set(0);
    move(1, 0);
}

void curses_end() {
	endwin();
}



