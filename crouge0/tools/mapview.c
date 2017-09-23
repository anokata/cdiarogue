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

struct IntPair calc_heightNwidth(char *filename) {
	ensure_file(filename);

	int height = 0, width = 0;
	char *content = read_whole_file(filename);
	char *it = content;

	while (*it != '\n') {
		width++;
		it++;
	}
	/* height++; */
	while (*it) {
		if (*it++ == '\n') height++;
	}
	
	free(content);
    struct IntPair p;
    p.a = width;
    p.b = height;
    return p;
}

void calc_hw(char *filename) {
    struct IntPair p = calc_heightNwidth(filename);
	printf("%d\n%d\n", p.a, p.b);
}

void set_map_data(char *map_name, char *map_data_file) {
	ensure_file(map_name);
	ensure_file(map_data_file);
    struct IntPair wh = calc_heightNwidth(map_data_file);

    printf("Set map data to %s from %s\n", map_name, map_data_file);
	char *content = read_whole_file(map_data_file);
    // TODO Refactor
    TileMap tmap = load_tile_map(map_name);
    Map map = make_map(wh.a, wh.b);
    /* tilemap_setchars(tmap, content); */
    // content drop \n
	printf("New w h %d:%d\n", wh.a, wh.b);
        size_t size = get_map_size(map);
        char *mapdata = malloc(size);
        memset(mapdata, ' ', size);
        int j = 0;
        for (size_t i = 0; i < strlen(content); i++) {
            if (content[i] != '\n') {
                mapdata[j++] = content[i];
            }
        }
        memcpy(map->data, mapdata, size);

    char newmapname[BUFSIZE];
    char *dup_map_name = strdup(map_name);
    char *name = basename(dup_map_name);
    snprintf(newmapname, BUFSIZE, "/tmp/%s.new", name);
    
    TileMap smap = map_convert2tilemap(map);
    tmap->width = wh.a;
    tmap->height = wh.b;
    void *tmp = tmap->tiles;
    tmap->tiles = smap->tiles;
    save_tilemap(tmap, newmapname);
        printf("Saved to %s\n", newmapname);
    
    tmap->tiles = tmp;

    free(mapdata);
    free(dup_map_name);
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
    {"view", show_map_cmd},
    {"hw", hw_calc_cmd},
    {"wh", hw_calc_cmd},
    {"setmap", set_map_data_cmd},
    {"mapset", set_map_data_cmd},
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
