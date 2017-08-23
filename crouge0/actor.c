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

