#include <ncurses.h>
#include <stdio.h>

#define MAX_WINDOWS 4
static int width;
static int heigth;
static WINDOW* wins[MAX_WINDOWS];

void init() {
	initscr();
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_YELLOW);
	init_pair(3, COLOR_GREEN, COLOR_WHITE);
	init_pair(4, COLOR_BLACK, COLOR_WHITE);
	init_pair(5, COLOR_WHITE, COLOR_BLACK);
	attron(COLOR_PAIR(4));
	refresh();
    width = getmaxx(stdscr);
    heigth = getmaxy(stdscr);
}

void end() {
	endwin();
	printf("%d:%d", width, heigth/2 - 1);
}

// setKeyHandler(void (*func)())
//chtype a = 'a' | A_BOLD | COLOR_PAIR(1);
void processInput() {
	int ch = getch();
	while(ch != 'q') {
		clear();
		if (ch != '\t');

		mvprintw(0, 0, "q = exit;");
		refresh();
		ch = getch();
	}
}

WINDOW* make_window() {
	WINDOW* win = newwin(heigth/1, width/2, 0, 0);
	wattron(win, 0);
	wbkgdset(win, COLOR_PAIR(4)  | ' ');
	wclear(win);
    return win;
}
void win1_refresh() {
    WINDOW* win1 = wins[0];
    box(win1, 0, 0);
    wmove(win1, 1, 1);
	wprintw(win1, "somenewwindowtex");
    refresh();
	wrefresh(win1);
}

void makeWins() {
	WINDOW* win1 = (make_window());
    wins[0] = win1;
    win1_refresh();
}

int main() {
    init();
    makeWins();
    processInput();
    end();
	return 0;
}
