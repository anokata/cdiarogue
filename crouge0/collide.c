#include "collide.h"

// Role of actor X Role of subject TODO END
ActionFunc actions[] = {

};

ActionFunc _get_action(Actor actor, Actor subject, G g) {
    UNUSED(actor);
    UNUSED(subject);
    //return actions[]
    return _no_action;
}

void _no_action(Actor actor, Actor subject, G g) {
    UNUSED(actor);
    UNUSED(subject);
    debuglog(g, "action undef");
}

void collision_effect(Actor actor, Actor subject, G g) {
    // TODO LAST
    // collide by roles
    // run_action(actor, subject);
    _get_action(actor, subject, g)(actor, subject, g);
    UNUSED(subject);
    if (actor->role == RolePlayer) {
        debuglog(g, "collide from player");
    } else {
        debuglog(g, "collide from monster");
    }
}

/* return collided actor */
Actor collision_get_actor(Actor actor, int dx, int dy, GList *actors) {
    GList *node = actors;
    int x = actor->x + dx;
    int y = actor->y + dy;

    while (node) {
        Actor subject = node->data;
        // also checks for self collide
        if ((x == subject->x) && (y == subject->y) && (actor != subject)) {
            return subject;
        }
        node = g_list_next(node);
    }
    return NULL;
}

/* True if can move */
bool collisions_player_move(Actor player, int dx, int dy, G g) {
    Actor subject = collision_get_actor(player, dx, dy, g->actors);
    if (!subject) {
        return actor_move_hv(player, g->gmap, dx, dy);
    } else {
        collision_effect(player, subject, g);
    }
    return false;
}
