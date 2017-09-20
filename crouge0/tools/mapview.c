#include <stdio.h>
#include <unistd.h>
#include "tile_map.h"
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


Command Commands[] = {
    {"show", show_map_cmd},
    {"hw", hw_calc_cmd},
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
