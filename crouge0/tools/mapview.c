#include <stdio.h>
#include "tile_map.h"
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

void show_map(char *map_name) {
    TileMap l = load_tile_map(map_name);
    print_tile_map(l);
    free_tile_map(l);
}

void show_map_cmd(int argc, char *argv[]) {
UNUSED(argc);
    char *map_name = argv[2];
	show_map(map_name);
}


Command Commands[] = {
    {"show", show_map_cmd},
    /* {"hw", calc_hw}, */
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
