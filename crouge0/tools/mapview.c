#include <stdio.h>
#include <unistd.h>
#include "tile_map.h"
#include "map_gen.h"
#include "util.h"
#define DEBUG

/* cmd */
#define MAX_CMD_NAME 32

typedef void (*CmdFunction)(int argc, char *argv[]);

typedef struct Command {
	char name[MAX_CMD_NAME];
    CmdFunction run;
} Command;

Command cmd_get(char *name);
/* cmd end */

void calc_hw(char *filename) {
	ensure_file(filename);

	int height = 0, width = 0;
	char *content = read_whole_file(filename);
	char *it = content;

	while (*it != '\n') {
		width++;
		it++;
	}
	height++;
	while (*it) {
		if (*it++ == '\n') height++;
	}
	
	free(content);
	printf("%d\n%d\n", width, height);
}

void set_map_data(char *map_name, char *map_data_file) {
    // !! TODO Recalc Width Height
	ensure_file(map_name);
	ensure_file(map_data_file);
    printf("Set map data to %s from %s\n", map_name, map_data_file);
	char *content = read_whole_file(map_data_file);
    // TODO Refactor
    TileMap tmap = load_tile_map(map_name);
    Map map = tilemap_convert2map(tmap);
    /* tilemap_setchars(tmap, content); */
    // content drop \n
        size_t size = get_map_size(map);
        char *mapdata = malloc(size);
        memset(mapdata, ' ', size);
        int j = 0;
        for (size_t i = 0; i < size; i++) {
            if (content[i] != '\n') {
                mapdata[j++] = content[i];
            }
        }
        memcpy(map->data, mapdata, size);

    char newmapname[BUFSIZE];
    snprintf(newmapname, BUFSIZE, "%s.new", map_name);
    
    TileMap smap = map_convert2tilemap(map);
    void *tmp = tmap->tiles;
    tmap->tiles = smap->tiles;
    save_tilemap(tmap, newmapname);
        printf("Saved to %s\n", newmapname);
    
    tmap->tiles = tmp;

    free(mapdata);
    free_tile_map(smap);
    free_tile_map(tmap);
    free_map(map);
    free(content);
}

void show_map(char *map_name) {
	ensure_file(map_name);
    TileMap l = load_tile_map(map_name);
    print_tile_map(l);
    free_tile_map(l);
}

void show_map_cmd(int argc, char *argv[]) {
UNUSED(argc);
    char *map_name = argv[2];
	show_map(map_name);
}


void hw_calc_cmd(int argc, char *argv[]) {
UNUSED(argc);
    char *map_name = argv[2];
	calc_hw(map_name);
}


void set_map_data_cmd(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Not enoght arguments\n");
        exit(1);
    }
    char *map_name = argv[2];
    char *map_data_file = argv[3];
    set_map_data(map_name, map_data_file);
}


Command Commands[] = {
    {"show", show_map_cmd},
    {"hw", hw_calc_cmd},
    {"setmap", set_map_data_cmd},
};
static int commands_count = sizeof(Commands)/sizeof(Command);

Command cmd_get(char *name) {
    for (int i = 0; i < commands_count; i++) {
        if (strcmp(Commands[i].name, name) == 0) {
            return Commands[i];
        }
    }
    return Commands[0];
}


void dispatch(int argc, char *argv[]) {
	Command cmd = cmd_get(argv[1]);
	cmd.run(argc, argv);
}

int main(int argc, char* argv[]) {
    UNUSED(argc);
    UNUSED(argv);
    DEBUG_PRINT("%d:", argc);
    DEBUG_PRINT("%s\n", argv[0]);

	if (argc > 1) {
		dispatch(argc, argv);
	} else {
		show_map("../maps/map_1_1");
    }
}
