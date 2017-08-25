#include "render.h"

void draw_actors(GList *actors, Viewport *v) {
    GList *it = actors;
    int top = viewport_top(v);
    int left = viewport_left(v);

    while (it) {
        Actor actor = it->data;
        if (in_viewport(v, actor->x, actor->y)) {
            draw_actor(it->data, 
                    v->display_left + actor->x - left, 
                    v->display_top + actor->y - top);
        }
        it = g_list_next(it);
    }
}

void draw_actor(Actor actor, int x, int y) {
    cc_putxy(actor->c, actor->color, x, y);
}

bool in_viewport(Viewport *v, int x, int y) {  // TODO union with Point struct
    int top = viewport_top(v);
    int left = viewport_left(v);
    int bottom = viewport_bottom(v);
    int right = viewport_right(v);
    return (x >= left) && (y >= top) && (x < right) && (y < bottom);
}

