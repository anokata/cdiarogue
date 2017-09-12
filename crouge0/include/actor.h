#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include <glib.h>
#include "map.h"
#include "item.h"
#include "util.h"

#define MAKE_STRING(STR) #STR,
#define MAKE_ENUM(X) X,

#define FOREACH_ROLE(ROLE) \
    ROLE(RolePlayer) \
    ROLE(RoleMonster) \
    ROLE(RoleNPC) \
    ROLE(RoleLength) \

typedef enum Role {
    FOREACH_ROLE(MAKE_ENUM)
} Role;

typedef enum Behavior { 
    BehaviorRandom=0, 
    BehaviorSimpleDirect, 
    BehaviorStand, 
    BehaviorSimpleAttacker 
} Behavior;

/* typedef enum Role { */ 
/*     RolePlayer, */ 
/*     RoleMonster, */ 
/*     RoleNPC, */
/*     RoleLength */
/* } Role; */

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
    Item equiped_right_hand;
    Item equiped_head;
    Item equiped_foot;
    Item equiped_legs;
    Item equiped_arms;
    Item equiped_body;
} *Actor;

CharPoint actor_as_charpoint_cast(Actor actor);

Actor make_actor(char c, int x, int y);
void free_actors(GList *actors);
void actor_free(Actor actor);
bool _actor_isat_directed_place(Actor actor);
int _actor_direct_diffx(Actor actor);
int _actor_direct_diffy(Actor actor);

int actor_stat_maxhp(Actor actor);
int actor_stat_attack(Actor actor); /* physical */
int actor_stat_defence(Actor actor); /* physical */
int actor_stat_regen(Actor actor);
int actor_calc_damage(Actor attacker, Actor defender);
void actor_heal(Actor actor, int value);

bool actor_equip(Actor actor, Item item);
bool actor_item_is_equiped(Actor actor, Item item);
Item *actor_item_slot(Actor actor, Item item);
