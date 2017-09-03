#include "behavior.h"

/* index = Behavior  value = move func */
static ActorMoveFunc actor_move_behavior_map[] = { 
    /* just random moving */
    actor_move_rand,
    /* choose direct point, move to it, if collided then random move one, 
     * at point choose other rand point. */
    actor_step_at_directed,
    actor_not_move,
    actor_simple_attack
};

/* Moving */
Point actor_get_move_point(Actor actor, TileMap map) {
    return actor_move_behavior_map[actor->behavior](actor, map);
}

/* Actual moving with change coords */
bool actor_move_hv(Actor actor, TileMap map, int h, int v) {
    if (is_passable(map, actor->x + h, actor->y + v)) {
            actor->x += h;
            actor->y += v;
            return true;
    }
    return false;
}

/* Behavior func */
Point actor_step_at_directed(Actor actor, TileMap map) {
    if (_actor_isat_directed_place(actor)) {
        _actor_choose_direct_point_rand(actor, map);
    }
    return _actor_move_direct(actor, map);
}

Point _actor_move_direct(Actor actor, TileMap map) {
    int horz_move = _actor_direct_diffx(actor);
    int vert_move = _actor_direct_diffy(actor);
    if (!is_passable(map, actor->x + horz_move, actor->y + vert_move)) {
        return actor_move_rand(actor, map);
    }
    Point p = {horz_move, vert_move};
    return p;
}

/* Behavior func */
Point actor_simple_attack(Actor actor, TileMap map) {
    /* hunt on player */
    Actor player = map_get_player(map);
    actor->directed.x = player->x;
    actor->directed.y = player->y;
    return _actor_move_direct(actor, map);
}

/* Behavior func */
Point actor_not_move(Actor actor, TileMap map) {
    UNUSED(actor);
    UNUSED(map);
    Point p = {0, 0};
    return p;
}

/* Behavior func */
Point actor_move_rand(Actor actor, TileMap map) {
    UNUSED(actor);
    UNUSED(map);
    // TODO Dice roll [-1, 1]
    int horz_move = rand() % 3 - 1; 
    int vert_move = rand() % 3 - 1;
    Point p = {horz_move, vert_move};
    return p;
}

void _actor_choose_direct_point_rand(Actor actor, TileMap map) {
    int x = rand() % map->width;
    int y = rand() % map->height;
    actor->directed.x = x;
    actor->directed.y = y;
}
