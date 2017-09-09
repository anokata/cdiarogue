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
    /* TODO weapon */
    return actor->basestat_strength * 2;
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
