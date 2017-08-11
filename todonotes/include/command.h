#pragma once
#define MAX_CMD_NAME 32

typedef void (*CmdFunction)(char **);

typedef struct Command {
	char name[MAX_CMD_NAME];
    CmdFunction run;
} Command;

Command cmd_get(char *name);

