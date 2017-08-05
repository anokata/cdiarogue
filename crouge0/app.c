#include "app.h"

enum States {State_run, State_end, State_wmap, State_cursor,  NUM_STATES};
enum Events {Event_draw, Event_key,  NUM_EVENTS};
/* GLOBAL */
State state;

// menu (not up down - use vi like.)
// ascii art pic editor mode, terminal again vi keys... + animation.
// Idea and world First! get old records and generate mechanic
// First - fun therefore slash like dia
// but not clone dia.
// 2. Magic and skill grows like morow
// 3. World height? it difficult for first try, lets it be simple dung with wild
// 4. Draw static map, weather zones, caves. Circle
// 5. fun events, fast grow, new challanges, FF, skills/items (keys, levitate, lava, swim) for access areas, bosses
// 6. end goal, meaning of all of this - survive and escape from planet. from black entitys.
// day part status - sun/moon weather.
// anim trees ...
// 1. World map, how store, view it with self position. map blocks. view frame.
//+1.1 Struct for wmap
//+1.2 Load wmap.
//+1.3 View wmap. (color regions) 
//+1.3.1 wmap mode.
// 1.4 show map region description.
// 1.5 gettext _
// 2.  local map, coords to wmap, load regions, moving, store, load
//.2.1 cursor move mode
// 2.2 how store/load local map? and view in edges. Viewport map. Load from pieces to global map array.
//+... local map files naming
//+2.3 load to global map
//+2.3.1 gmap mode
//+2.3.2 colors (without attributes)
// 2.4 global to viewport at point
// 2.5 moving and view map
// WRite and draw GP Mechanic, view, make questions and decisions, KNOW WHAT TO DO
// 1. walking @
// 2. debug and msg
// 3. simple location
// 4. walking @ loc stop walls
// 5. add monstr, simple ai, stay, rand. time steps
// 6. interacting, simple combat
// 7. items

// [ ] make wmap from locals
// UI progress bar with value
// unicode ncursesw
//http://www.roguebasin.com/index.php?title=Ncursesw
//https://stackoverflow.com/questions/43834833/print-a-wide-unicode-character-with-ncurses

void processInput(G g) {
    ss_handle(state, Event_draw, g);
	int ch = getch();
	while(ch != 'q') {
        g->key = ch;
        ss_handle(state, Event_key, g);
        ss_handle(state, Event_draw, g);
		ch = getch();
	}
}

int key_run(void* data) {
    G g = data;
    char key = g->key;
    cc_printi(key, cd_green);
    switch (key) {
        case 'w':
            ss_setstate(state, State_wmap);
            break;
        case 'c':
            ss_setstate(state, State_cursor);
            break;
    }
    return 0;
}

int draw(void* data) {
    clear();
    /* cc_putxy('D', cb_yellow, 3, 2); */
    cc_printxy(
            "Key bindings:\n\
        w world map\n\
        h this help\n\
        c cursor mode\n", 
            cn_white, 0, 0);
    return 0;
}

int wmap_key(void* data) {
    G g = data;
    char key = g->key;
    switch (key) {
        case 'h':
            ss_setstate(state, State_run);
            break;
    }
    return 0;
}

int wmap_draw(void* data) {
    G g = data;
    clear();
    cc_putxy('M', cb_yellow, 3, 2);
    print_wmap(g->wmap);
    return 0;
}

int cursor_key(void* data) {
    G g = data;
    char key = g->key;
    switch (key) {
        case 'r':
            ss_setstate(state, State_run);
            break;
        case 'j':
            g->cursor.y++;
            g->view->cy++;
            break;
        case 'k':
            g->cursor.y--;
            g->view->cy--;
            break;
        case 'h':
            g->cursor.x--;
            g->view->cx--;
            break;
        case 'l':
            g->cursor.x++;
            g->view->cx++;
            break;
    }
    return 0;
}

int cursor_draw(void* data) {
    G g = data;

    clear();
    /* cc_putxy(g->key, cb_yellow, 3, 2); */
    draw_map(g->gmap, g->view);
    cc_putxy(' ', cw_white, g->cursor.x, g->cursor.y);
    return 0;
}

void state_init() {
    state = ss_make_state(NUM_STATES, NUM_EVENTS);
    ss_sethander(state, State_run, Event_draw, draw);
    ss_sethander(state, State_run, Event_key, key_run);
    ss_sethander(state, State_wmap, Event_draw, wmap_draw);
    ss_sethander(state, State_wmap, Event_key, wmap_key);
    ss_sethander(state, State_cursor, Event_draw, cursor_draw);
    ss_sethander(state, State_cursor, Event_key, cursor_key);
    ss_setstate(state, State_run);
}

void start() {
    G g = new_g();
    /* free_g(g); */
    /* return; */
    curses_init();
    state_init();

    processInput(g);
    ss_free_state(state);
    curses_end();
    free_g(g);
}
