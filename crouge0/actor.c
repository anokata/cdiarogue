#include "actor.h"

Actor make_actor(char c, int x, int y) {
    Actor actor = malloc(sizeof(struct Actor));
    actor->c = c;
    actor->x = x;
    actor->y = y;
    actor->directed.x = x;
    actor->directed.y = y;
    actor->behavior = BehaviorRandom;
    actor->color = cn_white;
    actor->stat_attack = 1;
    actor->stat_hp = 2;
    actor->role = RoleMonster;
    actor->name = "poring";
    actor->status = StatusLive;
    actor->items = NULL;
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
