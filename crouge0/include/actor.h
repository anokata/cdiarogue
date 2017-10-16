#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include <glib.h>
#include "map.h"
#include "item.h"
#include "util.h"
#include "config_parser.h"

#define FOREACH_ROLE(ROLE) \
    ROLE(RolePlayer) \
    ROLE(RoleMonster) \
    ROLE(RoleNPC) \
    ROLE(RoleLength) \

typedef enum Role {
    FOREACH_ROLE(MAKE_ENUM)
} Role;

Role role_from_str(char *str);

#define FOREACH_BEHAVIOR(X) \
    X(BehaviorRandom) \
    X(BehaviorSimpleDirect) \
    X(BehaviorStand) \
    X(BehaviorSimpleAttacker) \

typedef enum Behavior {
    FOREACH_BEHAVIOR(MAKE_ENUM)
} Behavior;

#define FOREACH_STATUS(X) \
    X(StatusLive) \
    X(StatusDead) \
    X(StatusSleep) \

typedef enum Status {
    FOREACH_STATUS(MAKE_ENUM)
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
    int stat_mp;
    int basestat_constitution; /* con -> max hp */
    int basestat_strength; /* str -> attack */
    int basestat_dexterity; /* dex -> dodge, crit */
    int basestat_intelligence; /* int -> mana, magic attack */
    /* ? magic defence */
    long exp;
    int lvl;
    int stat_points;
    char *name;
    Role role;
    Items items;
    char *items_file;
    Item equiped_right_hand;
    Item equiped_head;
    Item equiped_foot;
    Item equiped_legs;
    Item equiped_arms;
    Item equiped_body;
} *Actor;

typedef GList *Actors;

CharPoint actor_as_charpoint_cast(Actor actor);

Actor make_actor(char c, int x, int y);
void free_actors(GList **actors);
Actor actor_clone(Actor actor);
void actor_add(GList **actors, Actor actor);
void actor_free(Actor actor);
bool _actor_isat_directed_place(Actor actor);
int _actor_direct_diffx(Actor actor);
int _actor_direct_diffy(Actor actor);

int actor_stat_maxhp(Actor actor);
int actor_stat_maxmp(Actor actor);
int actor_stat_attack(Actor actor); /* physical */
int actor_stat_defence(Actor actor); /* physical */
int actor_stat_regen(Actor actor);
int actor_stat_mp_regen(Actor actor);
int actor_calc_damage(Actor attacker, Actor defender);
int actor_weapon_dispersion(Actor actor);
void actor_heal(Actor actor, int value);
void actor_heal_mp(Actor actor, int value);
float actor_calc_dodge(Actor attacker, Actor defender);
float actor_stat_dodge(Actor actor);

bool actor_equip(Actor actor, Item item);
bool actor_takeoff(Actor actor, Item item);
bool actor_item_is_equiped(Actor actor, Item item);
Item *actor_item_slot(Actor actor, Item item);

/* serialization */
Actor actor_from_strings(Strings str);
char *actor_serialize(Actor actor);
GList *actors_load(char* filename);
void actors_save(char* filename, GList *actors);

Role role_from_str(char *str);
Behavior behavior_from_str(char *str);
Status status_from_str(char *str);
void actor_exp_gain(Actor actor, long exp);
void actor_lvl_up(Actor actor);

void test_exp();
long actor_defeat(Actor actor, Actor subject);
long calc_exp_gain(Actor actor, Actor subject);
void fill_exp_road();
bool is_lvl_up(int lvl, long new_exp);
long exp_get_to_next(int lvl);
