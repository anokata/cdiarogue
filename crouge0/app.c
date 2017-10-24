#include "app.h"
char version[] = "0.0.6";
/* save file: */
char *player_file = "./maps/you";
char *test_savefile = "./save/test.save";
char *savefile = "./save/test.save";
/* char *player_items_file = "./maps/you.items"; */

/* GLOBAL */
extern State state;

void ui_draw(G g);
void save_location(TileMap map);
void load(G g);
void g_post_init(G g);
void anim_start(G g);
void save_savefile(G g);

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
        "@ x:y = %d:%d %d$",
        g->player->x, g->player->y, actor_money_count(g->player)
        );
    cc_printxy(buf, cn_white, UI_X, y++);
    snprintf(buf, BUFSIZE, 
        "[HP: %d/%d MP: %d/%d atk: %d def: %d ]",
        g->player->stat_hp, actor_stat_maxhp(g->player), 
        g->player->stat_mp, actor_stat_maxmp(g->player), 
        actor_stat_attack(g->player), 
        actor_stat_defence(g->player)
        );
    cc_printxy(buf, cn_white, UI_X, y++);
    snprintf(buf, BUFSIZE, 
        "%s Turn: %d [exp: %ld  lvl: %d ]",
        sky_steps2time(g->turns), g->turns,
        g->player->exp, g->player->lvl
        );
    cc_printxy(buf, cn_white, UI_X, y++);

    snprintf(buf, BUFSIZE, 
        "Target [HP: %d ]\t next lvl exp: %ld\n",
        (g->last_target ? g->last_target->stat_hp : 0),
        exp_get_to_next(g->player->lvl)
    );
    cc_printxy(buf, cn_white, UI_X, y++);

    y++;
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
            "loc %s\n" 
        , g->key, 
        g->view->display_left, g->view->cx, g->view->cy,
        viewport_left(g->view), viewport_top(g->view),
        g->location_path
        );
    cc_printxy(buf, cn_white, 0, 20);
}

int draw(void* data) {
    UNUSED(data);
    clear();
    cc_printxy(
            "Key bindings:\n\
        ESQ exit\n\
        h this help\n\
        c character game mode\n\
        i show inventory\n\
        d drop an item \n\
        q quaff a potion \n\
        e equip something \n\
        , take item on ground \n\
        . rest one step \n\
        / debug info on/of\n\
        w world map\n\
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

static const char *special_locations[] = {
    "lvl1",
    "lvl2"
};

bool is_special_location(char *location_path) {
    for (size_t i = 0; i < sizeof(special_locations)/sizeof(char*); i++) {
        if (!strcmp(special_locations[i], location_path)) {
            return true;
        }
    }
    return false;
}

void change_map(G g, char *location_filename) {
    // save player

    char *location_filename_d = strdup(location_filename);

    if (is_special_location(location_filename)) {
        char *back_location_path = strdup(g->location_path);
        // generate map
        char *location_name = gen_location(back_location_path, 
                location_filename, location_filename);
        // and load it
        free(location_filename_d);
        location_filename_d = strdup(location_name);
    
        free(location_name);
        free(back_location_path);

    } else { }

    free_global_map(g->gmap);
    g->player = NULL;

    free(g->location_path);
    g->location_path = location_filename_d;
    save_savefile(g);
    free(g->location_path);
    load(g);
    g_post_init(g);
}

void enter_location(G g) {
    CharPoint cp = charpoint_at(g->gmap->objects, g->player->x, g->player->y);
    Object obj = (Object) cp;
        // if obj type portal
if (obj && (obj->type == ObjectTypeExit|| obj->type == ObjectTypeEnter)) {
        debuglog(g, obj->param);
        change_map(g, obj->param);
    } else {
        debuglog(g, "Can not enter down here");
    }
}

int cursor_key(void* data) {
    G g = data;
    char key = g->key;
    char *msg;
    switch (key) {
        case 'a':
            anim_start(g);
            break;
        case '>':
        case '<':
            enter_location(g);
            break;
        case 's':
            save_player(g->player, player_file);
            save_location(g->gmap);
            debuglog(g, "Saved");
            break;
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
        case '.':
            actor_heal(g->player, actor_stat_regen(g->player));
            break;
    }
    move_all_actors_rand(g);
    return proc_all_actors_status(g);
}

int cursor_draw(void* data) {
    G g = data;

    clear();
    map_lightning(g->gmap, g->player->x, g->player->y);
    draw_map(g->gmap, g->view);
    /* cc_putxy('@', cn_yellow, */ 
    /*         min(g->view->cx, g->view->width / 2) + g->view->display_left, */ 
    /*         min(g->view->cy, g->view->height / 2) + g->view->display_top); */
    draw_charpoints(g->gmap->items, g);
    draw_charpoints(g->gmap->actors, g);
    draw_charpoints(g->gmap->objects, g);
    draw_actor_self(g->player, g->view);
    ui_draw(g);
    return 0;
}

void anim_1(void *data) {
    G g = data;
    cc_putxy(g->anim->c++, cn_white, g->anim->sx++, 1);
}

void anim_start(G g) {
    ss_setstate(state, State_anim);
    g->anim = anim_make();
    g->anim->frame = 8;
    g->anim->fun = anim_1;
    g->anim->c = 'a';
}

bool anim_proc(G g) {
    bool is_end = anim_step(g->anim);
    usleep(100000);
    if (is_end) {
        anim_free(g->anim);
        g->anim = NULL;
    }
    return is_end;
}

int anim_draw(void* data) {
    G g = data;
    UNUSED(g);
    cursor_draw(g);
    while (!anim_proc(g)) {
        cursor_draw(g);
        g->anim->fun(g);
        cc_refresh();
    }
        ss_setstate(state, State_cursor);
    return 0;
}

int anim_key(void* data) {
    G g = data;
    UNUSED(g);
    /* if (anim_proc(g)) { */
    /*     ss_setstate(state, State_cursor); */
    /* } */
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
    ss_sethander(state, State_anim, Event_draw, anim_draw);
    ss_sethander(state, State_anim, Event_key, anim_key);
    ss_setstate(state, State_cursor);
    ss_setstate(state, State_charinfo);
}

void save_location(TileMap map) {
    items_save(map->items_file, map->items);
}

void save(G g) {
    //save_player(g->player, player_file); // unplug for debug 
    UNUSED(g);
}

void load(G g) {
    load_savefile(g);
}

void load_savefile(G g) {
    GHashTable *config = parse_file(g->savefile);
    char *player_file = g_hash_table_lookup(config, "player");
    char *location_path = g_hash_table_lookup(config, "location");

    load_player(&g->player, player_file);
    g->gmap = load_global_tmap(location_path);
    g->location_path = strdup(location_path);

    g_hash_table_destroy(config);
}

void save_savefile(G g) {
    char buf[BUFSIZE];
    snprintf(buf, BUFSIZE, "player:%s\nlocation:%s\n", player_file, g->location_path);
    FILE *fout = fopen(g->savefile, "w");
    fwrite(buf, strlen(buf), 1, fout);
    fclose(fout);
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
}

void item_debug(G g) {
    Item potion = item_new('!', 10, 10);
    potion->type = ItemPotionOfCure;
    Item hat = item_new(']', 12, 12);
    hat->type = ItemStrawHat;
    tmap_add_item(g->gmap, potion);
    tmap_add_item(g->gmap, hat);

    potion = item_new('1', 8, 8);
    potion->type = ItemPotionOfCure;
    potion->cls = ItemPotionCls;
    potion->name = strdup("potion #32 GEX UHT");
    item_add(&g->gmap->items, potion);
}

void g_post_init(G g) {
    /* move player to exit */
    Object exit = objects_get_exit(g->gmap->objects);
    if (!exit) {
        perror("Can't find exit on map");
    }
    g->player->x = exit->x;
    g->player->y = exit->y;
    g->view->cx = g->player->x;
    g->view->cy = g->player->y;

    add_actor(g, g->player);
    /* init_inventory(g); */
}

G g_init(char *arg1)  {
    char *savefile_path = "./save/test.save";
    if (arg1) {
        savefile_path = arg1;
    }
    /* tests if debug */

    fill_exp_road();
    G g = new_g(savefile_path);
    g->wmap = load_wmap();
    load(g);
    g_post_init(g);

    return g;
}

void start(char *arg1) {
    G g = g_init(arg1);
    curses_init();
    state_init();

    /* actor_debug(g); */
    /* item_debug(g); */

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
