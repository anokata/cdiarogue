#include "app.h"
char version[] = "0.0.3";
char *player_file = "./maps/you";

/* GLOBAL */
extern State state;

void ui_draw(G g);

void process_input(G g) {
    ss_handle(state, Event_draw, g);
    debug_draw(g);
    ui_draw(g);
	int ch = getch();
    int status = 0;
	while(ch != 27 && !status) {
        g->key = ch;
        status = ss_handle(state, Event_key, g);
        ss_handle(state, Event_draw, g);
        proc_global_events(g);

        if (g->debug) {
            debug_draw(g);
        }
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

void ui_draw(G g) {
#define UI_X 50
    int log_end = g->log_len;
    int log_start = (g->log_len > 8) ? g->log_len - 8 : 0;

    int y = 1;

    char buf[BUFSIZE];
    snprintf(buf, BUFSIZE, 
        "@ x:y = %d:%d [HP: %d/%d atk: %d def: %d  exp: %ld  lvl: %d ]",
        g->player->x, g->player->y, g->player->stat_hp,
        actor_stat_maxhp(g->player), actor_stat_attack(g->player), 
        actor_stat_defence(g->player),
        g->player->exp, g->player->lvl
        );
    cc_printxy(buf, cn_white, UI_X, y++);

    snprintf(buf, BUFSIZE, 
        "Target [HP: %d ]\t next lvl exp: %ld\n",
        (g->last_target ? g->last_target->stat_hp : 0),
        exp_get_to_next(g->player->lvl)
    );
    cc_printxy(buf, cn_white, UI_X, y++);

    for (int i = log_start; i < log_end; i++) {
        char *msg = g_list_nth_data(g->log, i);
        cc_printxy(msg, cn_white, UI_X, y++);
    }
}

void debug_draw(G g) {
    char buf[1024];
    g_debug("hi");
    g_log(NULL, G_LOG_LEVEL_DEBUG, "MSG");

    snprintf(buf, 1024, 
            "Debug: \t"
            "Key: %d\n" 
            "VLeft: %d\t" 
            "Vcx:cy = %d:%d\t" 
            "Vleft:top = %d:%d\n" 
        , g->key, 
        g->view->display_left, g->view->cx, g->view->cy,
        viewport_left(g->view), viewport_top(g->view)
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
        c character game mode\n\
        i show inventory\n\
        d drop an item \n\
        q quaff a potion \n\
        e equip something \n\
        / debug info on/of\n\
        ", 
            cn_white, 0, 0);
    return 0;
}

// TODO mechanic Module?
void move_all_actors_rand(G g);

void move_all_actors_rand(G g) {
    GList *actor_node = g->gmap->actors;
    while (actor_node) {
        Actor actor = actor_node->data;
        // get point where actor want go
        Point next_point = actor_get_move_point(actor, g->gmap);
        // check for collide and go
        collisions_player_move(actor, next_point.x, next_point.y, g);
        actor_node = g_list_next(actor_node);
    }
}

int proc_all_actors_status(G g);
int proc_all_actors_status(G g) {
    GList *actor_node = g->gmap->actors;
    while (actor_node) {
        Actor actor = actor_node->data;
        actor_node = g_list_next(actor_node);
        if (actor->status == StatusDead) {
            // 
        }
    }
    if (g->player->status == StatusDead) {
        debuglog(g, "end");
        return 1;
    }
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
    char *msg;
    switch (key) {
        case '/':
            g->debug = !g->debug;
            break;
        case 'r':
            ss_setstate(state, State_run);
            break;
        case 'i':
            ss_setstate(state, State_inventory);
            break;
        case 'd':
            ss_setstate(state, State_drop); // TODO ?inventor action state with param
            /* array of [char, help, state, draw/key func] */
            break;
        case 'e':
            ss_setstate(state, State_equip);
            break;
        case 'q':
            ss_setstate(state, State_quaff);
            break;
        case 'c':
            ss_setstate(state, State_charinfo);
            break;
        case 't':
            ss_setstate(state, State_takeoff);
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
        case ',':
            // TODO make return description string
            msg = actor_take_from(g->player, g->gmap, g->player->x, g->player->y);
            debuglog(g, msg);
            break;
    }
    move_all_actors_rand(g);
    return proc_all_actors_status(g);
}

int cursor_draw(void* data) {
    G g = data;

    clear();
    draw_map(g->gmap, g->view);
    /* cc_putxy('@', cn_yellow, */ 
    /*         min(g->view->cx, g->view->width / 2) + g->view->display_left, */ 
    /*         min(g->view->cy, g->view->height / 2) + g->view->display_top); */
    draw_charpoints(g->gmap->items, g->view);
    draw_charpoints(g->gmap->actors, g->view);
    draw_actor_self(g->player, g->view);
    ui_draw(g);
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
    ss_sethander(state, State_inventory, Event_draw, inventory_draw);
    ss_sethander(state, State_inventory, Event_key, inventory_key);
    ss_sethander(state, State_drop, Event_draw, inventory_draw);
    ss_sethander(state, State_drop, Event_key, inventory_key);
    ss_sethander(state, State_quaff, Event_draw, inventory_draw);
    ss_sethander(state, State_quaff, Event_key, inventory_key);
    ss_sethander(state, State_equip, Event_draw, inventory_draw);
    ss_sethander(state, State_equip, Event_key, inventory_key);
    ss_sethander(state, State_takeoff, Event_draw, inventory_draw);
    ss_sethander(state, State_takeoff, Event_key, inventory_key);
    ss_sethander(state, State_charinfo, Event_draw, charinfo_draw);
    ss_sethander(state, State_charinfo, Event_key, charinfo_key);
    ss_setstate(state, State_cursor);
}

void save_player(Actor you) {
    extern char actor_file_header[];
    extern char actor_file_type[];
    FILE *out = fopen(player_file, "w+");
    fwrite(actor_file_header, strlen(actor_file_header), 1, out);
    fwrite(actor_file_type, strlen(actor_file_type), 1, out);

    char *line = actor_serialize(you);
    fwrite(line, strlen(line), 1, out);
    free(line);

    fclose(out);
}

void load_player(Actor *you) {
    if (*you) actor_free(*you);
    GList *lst = actors_load(player_file);
    Actor actor = lst->data;
    if (actor) {
        *you = actor;
    } // else create player?
    g_list_free(lst);
}

void save(G g) {
    // TODO
    save_player(g->player); // unplug for debug 
}

void load(G g) {
    load_player(&g->player);
    g->view->cx = g->player->x;
    g->view->cy = g->player->y;
}

void init_inventory(G g) {
    Item potion = item_new('!', 1, 1);
    potion->type = ItemPotionOfCure;
    potion->cls = ItemPotionCls;
    item_add(&g->player->items, potion);
    potion = item_new('!', 1, 1);
    potion->type = ItemPotionOfCure;
    potion->cls = ItemPotionCls;
    item_add(&g->player->items, potion);

    Item sword = item_new('/', 0, 0);
    sword->cls = ItemWeaponCls;
    sword->type = ItemWeaponSword;
    item_add(&g->player->items, sword);

    Item helm = item_new(']', 0, 0);
    helm->cls = ItemHeadEquipCls;
    helm->type = ItemStrawHat;
    item_add(&g->player->items, helm);
}


void actor_debug(G g) {
    Actor a = make_actor('b', 2, 2);
    a->name = strdup("bug");
    add_actor(g, a);
    a->behavior = BehaviorSimpleDirect;
    a->color = cb_blue;
    a->directed.x = 2;
    a->directed.y = 2;
    for (int i = 0; i < 2; i++) {
        a = make_actor('d', i + 15, 15);
        add_actor(g, a);
        a->color = cb_red;
        a->name = strdup("bug");
        a->behavior = BehaviorSimpleAttacker;
    }
    /* for (int i = 0; i < 20; i++) { */
    /*     a = make_actor('s', i, 5); */
    /*     a->color = cb_yellow; */
    /*     a->name = strdup("bug"); */
    /*     add_actor(g, a); */
    /* } */
}

void item_debug(G g) {
    Item potion = item_new('!', 10, 10);
    potion->type = ItemPotionOfCure;
    Item hat = item_new(']', 12, 12);
    hat->type = ItemStrawHat;
    tmap_add_item(g->gmap, potion);
    tmap_add_item(g->gmap, hat);
}

void start() {
    /* tests if debug */
        // TODO
    fill_exp_road();
    G g = new_g();
    load(g);
    add_actor(g, g->player);
    init_inventory(g);

    curses_init();
    state_init();

    actor_debug(g);
    item_debug(g);

    start_events();
    event_register(ActionCollide, RolePlayer, RoleMonster, collide_action_player_monster);
    event_register(ActionCollide, RoleMonster, RolePlayer, collide_action_monster_player);
    process_input(g);
    // End
    save(g);

    ss_free_state(state);
    curses_end();
    end_events();
    free_g(g);
}
