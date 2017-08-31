#include "collide.h"

void collision_effect(Actor actor, Actor subject, G g) {
    // TODO LAST
    // collide by roles
    // run_action(actor, subject);
    ActionFunc f = event_get(ActionCollide, actor->role, subject->role, g);
    if (f) {
        f(ActionCollide, actor, subject, g);
    } else {
        _no_action(ActionCollide, actor, subject, g);
    }

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

void collide_action_player_monster(Action action, Actor actor, Actor subject, G g) {
    UNUSED(actor);
    UNUSED(subject);
    UNUSED(action);
    char msg[100];

    snprintf(msg, 99, "%s hit %s by %dpt. remain HP:%d", actor->name, subject->name, actor->stat_attack, subject->stat_hp);
    debuglog(g, msg);
    /* mechanic */
    g->last_target = subject;
    subject->stat_hp -= actor->stat_attack;
    if (subject->stat_hp <= 0) {
        snprintf(msg, 99, "%s kill %s", actor->name, subject->name);
        debuglog(g, msg);

        //actor_kill(subject, g);
        subject->status = StatusDead;
        g->actors = g_list_remove(g->actors, subject);
        actor_free(subject); // strange behavior of g_list_remove (its free data)
    }

}

void collide_action_monster_player(Action action, Actor actor, Actor subject, G g) {
    UNUSED(actor);
    UNUSED(subject);
    UNUSED(action);
    debuglog(g, "! monster hit player");
}
