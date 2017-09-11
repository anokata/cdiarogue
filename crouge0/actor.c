#include "actor.h"
extern EError global_error;

Actor make_actor(char c, int x, int y) {
    Actor actor = malloc(sizeof(struct Actor));
    actor->c = c;
    actor->x = x;
    actor->y = y;
    actor->directed.x = x;
    actor->directed.y = y;
    actor->behavior = BehaviorRandom;
    actor->color = cn_white;
    actor->role = RoleMonster;
    actor->name = "poring";
    actor->status = StatusLive;
    actor->items = NULL;
    actor->basestat_constitution = 1;
    actor->basestat_strength = 1;
    actor->stat_hp = actor_stat_maxhp(actor);
    actor->equiped_right_hand = NULL;
    actor->equiped_head = NULL;
    actor->equiped_foot = NULL;
    actor->equiped_legs = NULL;
    actor->equiped_arms = NULL;
    actor->equiped_body = NULL;
    return actor;
}

void actor_free(Actor actor) {
    items_free(&actor->items);
    free(actor);
}

void free_actors(GList *actors) {
    while (actors) {
        Actor actor = actors->data;
        actor_free(actor);
        actors = g_list_next(actors);
    }
}

bool _actor_isat_directed_place(Actor actor) { 
    return (actor->x == actor->directed.x) && (actor->y == actor->directed.y);
}

int _actor_direct_diffx(Actor actor) {
    int diff = (actor->directed.x - actor->x);
    return diff == 0 ? 0 : (diff > 0 ? 1 : -1);
}

int _actor_direct_diffy(Actor actor) {
    int diff = (actor->directed.y - actor->y);
    return diff == 0 ? 0 : (diff > 0 ? 1 : -1);
}

CharPoint actor_as_charpoint_cast(Actor actor) {
    return (struct CharPoint *) actor;
}

int actor_stat_maxhp(Actor actor) {
    return actor->basestat_constitution * 10;
}

int actor_stat_attack(Actor actor) {
    int atk = actor->basestat_strength * 2;
    if (actor->equiped_right_hand) atk += actor->equiped_right_hand->value;
    return atk;
}

int actor_stat_defence(Actor actor) {
    int def = 0;
    if (actor->equiped_head) def += actor->equiped_head->value;
    if (actor->equiped_foot) def += actor->equiped_foot->value;
    if (actor->equiped_legs) def += actor->equiped_legs->value;
    if (actor->equiped_arms) def += actor->equiped_arms->value;
    if (actor->equiped_body) def += actor->equiped_body->value;
    return def;
}

int actor_calc_damage(Actor attacker, Actor defender) {
    int atk = actor_stat_attack(attacker);
    int def = actor_stat_defence(defender);
    int dif = atk - def;
    return MAX(dif, 0);
}

void actor_heal(Actor actor, int value) {
    actor->stat_hp = (actor->stat_hp + value) % (actor_stat_maxhp(actor) + 1);
}

bool actor_equip(Actor actor, Item item) {
    switch (item->cls) {
        case ItemWeaponCls:
            if (actor->equiped_right_hand) {
                global_error = Error_Actor_SlotBusy;
                return false;
            }
            actor->equiped_right_hand = item;
            return true;
            break;
        case ItemHeadEquipCls:
            if (actor->equiped_head) {
                global_error = Error_Actor_SlotBusy;
                return false;
            }
            actor->equiped_head = item;
            return true;
            break;
        default:
            global_error = Error_Actor_NotEquptable;
            return false;
            break;
    }
    return false;
}

bool actor_item_is_equiped(Actor actor, Item item) {
    if (actor->equiped_right_hand == item) return true;
    if (actor->equiped_head == item) return true;
    if (actor->equiped_foot == item) return true;
    if (actor->equiped_legs == item) return true;
    if (actor->equiped_arms == item) return true;
    if (actor->equiped_body == item) return true;
    /* TODO slots to array with enum? */
    return false;
}

Item *actor_item_slot(Actor actor, Item item) {
    if (actor->equiped_right_hand == item) return &actor->equiped_right_hand;
    if (actor->equiped_head == item) return &actor->equiped_head;
    if (actor->equiped_foot == item) return &actor->equiped_foot;
    if (actor->equiped_legs == item) return &actor->equiped_legs;
    if (actor->equiped_arms == item) return &actor->equiped_arms;
    if (actor->equiped_body == item) return &actor->equiped_body;
    return NULL;
}
