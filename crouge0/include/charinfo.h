#pragma once
#include "lib/ccurses.h"
#include "g.h"

int charinfo_key(void* data);
int charinfo_draw(void* data);
char *charinfo_print(Actor player); /* return static str */
