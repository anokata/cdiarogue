#include "location.h"
static char *player_file = "./maps/you";

void save_location(TileMap map) {
    items_save(map->items_file, map->items);
}

void save(G g) {
    save_player(g->player, player_file);
    save_location(g->gmap);
    /* TODO save mobs, savefile */
}

void load(G g) {
    load_savefile(g);

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

void change_map(G g, char *location_filename) {
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

    return g;
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

