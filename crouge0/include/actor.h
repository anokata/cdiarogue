#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include <glib.h>
#include "map.h"

typedef enum Behavior { 
    BehaviorRandom=0, 
    BehaviorSimpleDirect, 
    BehaviorStand, 
    BehaviorSimpleAttacker 
} Behavior;

typedef enum Role { 
    RolePlayer, 
    RoleMonster, 
    RoleNPC,
    RoleLength
} Role;

typedef enum Status {
    StatusLive,
    StatusDead,
    StatusSleep
} Status;

typedef struct Actor {
    char c;
    int x;
    int y;
    Behavior behavior;
    Point directed;
    Color color;
    Status status;
    int stat_attack;
    int stat_hp;
    char *name;
    Role role;
} *Actor;


Actor make_actor(char c, int x, int y);
void free_actors(GList *actors);
void actor_free(Actor actor);
bool _actor_isat_directed_place(Actor actor);
int _actor_direct_diffx(Actor actor);
int _actor_direct_diffy(Actor actor);
