#include "actor.h"

Actor make_actor(char c, int x, int y) {
    Actor actor = malloc(sizeof(struct Actor));
    actor->c = c;
    actor->x = x;
    actor->y = y;
    return actor;
}

void free_actors(GList *actors) {
    while (actors) {
        free(actors->data);
        actors = g_list_next(actors);
    }
}

void actor_move_rand(Actor actor);
// TODO if (is_passable())
    int horz_move = rand() % 3;
    int vert_move = rand() % 3;

    if (horz_move == 1) {
        actor->x++;
    } else if (horz_move == 2) {
        actor->x--;
    }

    if (vert_move == 1) {
        actor->y++;
    } else if (vert_move == 2) {
        actor->y--;
    }
}
