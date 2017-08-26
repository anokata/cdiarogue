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
//+5. add monstr, simple ai, stay, rand. time steps
//   5.1+list of actors, draw 
//   5.2+moving other by steps, rand, direct to near rand point, wall block 
//   5.3+actors attributes, color, attack, behavior
// [ INWORK ]
//~6. interacting, simple combat
//   6.0+player must be actor
//   6.1~actor-player collide detect, from player and from actors, events map
//   6.2 collide events by types. health, wounds
//
//   6.4 direct to player
// 7. items
//   7.1 doors, open, close
//   7.2 weapon, weild, take off. effect on combat
// ...
// fun
// levels dungs town
// drop from actors
//	Display turns.

// [ ] make wmap from locals
// UI progress bar with value
// unicode ncursesw
// save load
// data files for actors, items
// magic, skills, exp
//http://www.roguebasin.com/index.php?title=Ncursesw
//https://stackoverflow.com/questions/43834833/print-a-wide-unicode-character-with-ncurses
//	lighting
// Check map folder exist
// TODO fixing mem errors
// ATTENTION: Naming convention for func = <modulename>_<action>_<opt>

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
"Player x:y = %d:%d\n" 
        , g->key, log_text, 
        g->view->display_left, g->view->cx, g->view->cy,
        viewport_left(g->view), viewport_top(g->view),
        g->player->x, g->player->y
        );
    cc_printxy(buf, cn_white, 0, 20);
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

// TODO mechanic Module?
void move_all_actors_rand(G g);
bool actor_try_go(Actor actor, Point p, G g);

bool actor_try_go(Actor actor, Point p, G g) {
    // TODO
    // check map
    // collisions_player_move
    return true;
}

void move_all_actors_rand(G g) {
    GList *actor_node = g->actors;
    while (actor_node) {
        // TODO collide
        Actor actor = actor_node->data;

      actor_move(actor, g->gmap);
        // TODO-ing
        // get point where actor want go
        Point next_point = actor_get_move_point(actor, g->gmap);
        // check for collide
        if (actor_try_go(actor, next_point, g)) {
            // go if can
            // not need if collisions_player_move
            // actor_move2point(actor, next_point);
        }
        // ---
        actor_node = g_list_next(actor_node);
    }
}

bool collisions_player_move(Actor player, int dx, int dy, G g);
Actor collision_get_actor(Actor actor, int dx, int dy, GList *actors);
void collision_effect(Actor actor, Actor subject, G g);

void collision_effect(Actor actor, Actor subject, G g) {
    if (actor->role == RolePlayer) {
        debuglog(g, "collide from player");
    } else {
        debuglog(g, "collide from monster");
    }
}

/* return collided actor */
Actor collision_get_actor(Actor actor, int dx, int dy, GList *actors) {
    GList *node = actors;
    int x = actor->x + dx;
    int y = actor->y + dy;

    while (node) {
        Actor subject = node->data;
        if ((x == subject->x) && (y == subject->y)) {
            return subject;
        }
        node = g_list_next(node);
    }
    return NULL;
}

/* True if can move */
bool collisions_player_move(Actor player, int dx, int dy, G g) {
    Actor subject = collision_get_actor(player, dx, dy, g->actors);
    if (!subject) {
        return actor_move_hv(player, g->gmap, dx, dy);
    } else {
        collision_effect(player, subject, g);
    }
    return false;
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
            if (collisions_player_move(g->player, 0, 1, g)) {
                viewport_move_down(g->view, g->gmap);
            }
            break;
        case 'k':
            g->cursor.y--;
            if (collisions_player_move(g->player, 0, -1, g)) {
                viewport_move_up(g->view, g->gmap);
            }
            break;
        case 'h':
            g->cursor.x--;
            if (collisions_player_move(g->player, -1, 0, g)) {
                viewport_move_left(g->view, g->gmap);
            }
            break;
        case 'l':
            g->cursor.x++;
            if (collisions_player_move(g->player, 1, 0, g)) {
                viewport_move_right(g->view, g->gmap);
            }
            break;
        case 'y':
            if (collisions_player_move(g->player, -1, -1, g)) {
                viewport_move_leftup(g->view, g->gmap);
            }
            break;
        case 'u':
            if (collisions_player_move(g->player, 1, -1, g)) {
                viewport_move_rightup(g->view, g->gmap);
            }
            break;
        case 'm':
            if (collisions_player_move(g->player, 1, 1, g)) {
                viewport_move_rightdown(g->view, g->gmap);
            }
            break;
        case 'n':
            if (collisions_player_move(g->player, -1, 1, g)) {
                viewport_move_leftdown(g->view, g->gmap);
            }
            break;
    }
    move_all_actors_rand(g);
    return 0;
}

void draw_actor_self(Actor actor, Viewport *v) {
    int top = v->display_left - viewport_top(v);
    int left = v->display_top - viewport_left(v);
    draw_actor(actor, left + actor->x, top + actor->y);
}

int cursor_draw(void* data) {
    G g = data;

    clear();
    draw_map(g->gmap, g->view);
    /* cc_putxy('@', cn_yellow, */ 
    /*         min(g->view->cx, g->view->width / 2) + g->view->display_left, */ 
    /*         min(g->view->cy, g->view->height / 2) + g->view->display_top); */
    draw_actors(g->actors, g->view);
    draw_actor_self(g->player, g->view);
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
    Actor a = make_actor('o', 2, 2);
    add_actor(g, a);
    a->behavior = BehaviorSimpleDirect;
    a->behavior = BehaviorStand;
    a->color = cb_blue;
    a->directed.x = 5;
    a->directed.y = 1;
    /* for (int i = 0; i < 20; i++) { */
    /*     a = make_actor('d', i, 5); */
    /*     add_actor(g, a); */
    /*     a->color = cb_red; */
    /*     a->behavior = BehaviorSimpleDirect; */
    /* } */
    /* for (int i = 0; i < 20; i++) { */
    /*     a = make_actor('s', i, 5); */
    /*     a->color = cb_yellow; */
    /*     add_actor(g, a); */
    /* } */

    process_input(g);
    ss_free_state(state);
    curses_end();
    free_g(g);
}
