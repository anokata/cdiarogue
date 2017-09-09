#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include <glib.h>
#include "map.h"
#include "item.h"
#include "util.h"

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
    char c; /* First (four) fields is CharPoint struct */
    int x; /* Do not change it self or its order */
    int y; /* Access for it via cast function */
    Color color; /* end of CharPoint */
    Behavior behavior;
    Point directed;
    Status status;
    int stat_hp;
    int basestat_constitution; /* con -> max hp */
    int basestat_strength; /* str -> attack */
    char *name;
    Role role;
    Items items;
} *Actor;

CharPoint actor_as_charpoint_cast(Actor actor);

Actor make_actor(char c, int x, int y);
void free_actors(GList *actors);
void actor_free(Actor actor);
bool _actor_isat_directed_place(Actor actor);
int _actor_direct_diffx(Actor actor);
int _actor_direct_diffy(Actor actor);

int actor_stat_maxhp(Actor actor);
int actor_stat_attack(Actor actor);
void actor_heal(Actor actor, int value);
