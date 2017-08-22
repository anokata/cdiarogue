#pragma once
#include <stdlib.h>

typedef struct Actor {
    char c;
    int x;
    int y;
} *Actor;

Actor make_actor(char c, int x, int y);


