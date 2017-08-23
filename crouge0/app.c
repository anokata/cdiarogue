#include "app.h"
char version[] = "0.0.1";

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
//+2.4 global to viewport at point
//+2.5 moving and view map
// WRite and draw GP Mechanic, view, make questions and decisions, KNOW WHAT TO DO
// 1.+walking @
// 2.+debug and msg
// 3.+simple location
// 4.+walking @ loc stop walls
//  -4.1 stop at map ends/ now just for 0 left top
//  -4.2 map objects properties
//	+	load map with kv params format
//
// 5.~add monstr, simple ai, stay, rand. time steps
//   5.1+list of actors, draw 
//   5.2~moving other by steps, rand, direct to near rand point [ INWORK ]
//   5.3 actors attributes, color, attack, behavior
// 6. interacting, simple combat
// 7. items
// ...
// fun
// levels dungs town

// [ ] make wmap from locals
// UI progress bar with value
// unicode ncursesw
//http://www.roguebasin.com/index.php?title=Ncursesw
//https://stackoverflow.com/questions/43834833/print-a-wide-unicode-character-with-ncurses
// Check map folder exist
// TODO fixing mem errors

void process_input(G g) {
    ss_handle(state, Event_draw, g);
	int ch = getch();
	while(ch != 'q') {
        g->key = ch;
        ss_handle(state, Event_key, g);
        ss_handle(state, Event_draw, g);
        debug_draw(g);
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

void debug_draw(G g) {
    char buf[1024];
    // LOG
    char log_text[1024];
    log_text[0] = '\0';
    /* debuglog(g, "abc"); */
    g_debug("hi");
    g_log(NULL, G_LOG_LEVEL_DEBUG, "MSG");
    for (int i = 0; i < (g->log_len > 10 ? 10 : g->log_len); i++) {
        strcat(log_text, g_list_nth_data(g->log, i));
        strcat(log_text, "\n");
    }

    snprintf(buf, 1024, 
"Debug: \n"
"Key: %d\n" 
"Log:\n%s\n" 
"Viewport Left: %d\n" 
"Viewport cx:cy = %d:%d\n" 
"Viewport left:top = %d:%d\n" 
        , g->key, log_text, 
        g->view->display_left, g->view->cx, g->view->cy,
        viewport_left(g->view), viewport_top(g->view));
    cc_printxy(buf, cn_white, 0, 30);
}

int draw(void* data) {
    UNUSED(data);
    clear();
    cc_printxy(
            "Key bindings:\n\
        w world map\n\
        h this help\n\
        c cursor mode\n", 
            cn_white, 0, 0);
    return 0;
}

bool in_viewport(Viewport *v, int x, int y) {  // TODO union with Point struct
    int top = viewport_top(v);
    int left = viewport_left(v);
    int bottom = viewport_bottom(v);
    int right = viewport_right(v);
    return (x >= left) && (y >= top) && (x < right) && (y < bottom);
}

void draw_actors(GList *actors, Viewport *v);
void draw_actor(Actor actor, int x, int y);

void draw_actor(Actor actor, int x, int y) {
    cc_putxy(actor->c, cn_blue, x, y);
}
void draw_actors(GList *actors, Viewport *v) {
    GList *it = actors;
    int top = viewport_top(v);
    int left = viewport_left(v);

    while (it) {
        Actor actor = it->data;
        if (in_viewport(v, actor->x, actor->y)) {
            draw_actor(it->data, 
                    v->display_left + actor->x - left, 
                    v->display_top + actor->y - top);
        }
        it = g_list_next(it);
    }
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
            viewport_move_down(g->view, g->gmap);
            break;
        case 'k':
            g->cursor.y--;
            viewport_move_up(g->view, g->gmap);
            break;
        case 'h':
            g->cursor.x--;
            viewport_move_left(g->view, g->gmap);
            break;
        case 'l':
            g->cursor.x++;
            viewport_move_right(g->view, g->gmap);
            break;
    }
    return 0;
}

int cursor_draw(void* data) {
    G g = data;

    clear();
    draw_map(g->gmap, g->view);
    /* cc_putxy(' ', cw_white, g->cursor.x, g->cursor.y); */
    cc_putxy('@', cn_yellow, 
            min(g->view->cx, g->view->width / 2) + g->view->display_left, 
            min(g->view->cy, g->view->height / 2) + g->view->display_top);
    draw_actors(g->actors, g->view);
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
    curses_init();
    state_init();
    Actor a = make_actor('d', 2, 2);
    add_actor(g, a);
    a = make_actor('d', 5, 5);
    add_actor(g, a);

    process_input(g);
    ss_free_state(state);
    curses_end();
    free_g(g);
}
