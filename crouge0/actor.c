#include "actor.h"

/* index = Behavior  value = move func */
static ActorMoveFunc actor_move_behavior_map[] = { 
    actor_move_rand,
    actor_move_direct
};

Actor make_actor(char c, int x, int y) {
    Actor actor = malloc(sizeof(struct Actor));
    actor->c = c;
    actor->x = x;
    actor->y = y;
    actor->directed.x = x;
    actor->directed.y = y;
    actor->behavior = BehaviorRandom;
    return actor;
}

void free_actors(GList *actors) {
    while (actors) {
        free(actors->data);
        actors = g_list_next(actors);
    }
}

/* Moving */
void actor_move(Actor actor, TileMap map) {
    actor_move_behavior_map[actor->behavior](actor, map);
}

bool actor_isat_directed_place(Actor actor) { // TODO is priv? rename 2_
    return (actor->x == actor->directed.x) && (actor->y == actor->directed.y);
}

int actor_direct_diffx(Actor actor) {
    int diff = (actor->directed.x - actor->x);
    return diff == 0 ? 0 : (diff > 0 ? 1 : -1);
}

int actor_direct_diffy(Actor actor) {
    int diff = (actor->directed.y - actor->y);
    return diff == 0 ? 0 : (diff > 0 ? 1 : -1);
}

void actor_step_at_directed(Actor actor) {
    if (!actor_isat_directed_place(actor)) {
        actor->x += actor_direct_diffx(actor);
        actor->y += actor_direct_diffy(actor);
    }
}

void actor_move_hv(Actor actor, TileMap map, int h, int v) {
    if (is_passable(map, actor->x + h, actor->y + v)) {
            actor->x += h;
            actor->y += v;
    }
}

void actor_move_direct(Actor actor, TileMap map) {
    int horz_move = actor_direct_diffx(actor);
    int vert_move = actor_direct_diffy(actor);
    actor_move_hv(actor, map, horz_move, vert_move);
}

void actor_move_rand(Actor actor, TileMap map) {
    // TODO Dice roll [-1, 1]
    int horz_move = rand() % 3 - 1; 
    int vert_move = rand() % 3 - 1;
    actor_move_hv(actor, map, horz_move, vert_move);
}

void actor_choose_direct_point_rand(Actor actor, TileMap map) {
    // TODO END
}
/* moving end */


