#include "render.h"

void draw_charpoints(GList *elements, G g) {
    GList *it = elements;
    Viewport *v = g->view;
    int top = viewport_top(v);
    int left = viewport_left(v);

    while (it) {
        CharPoint cp = it->data;
        if (in_viewport(v, cp->x, cp->y)) {
            if (g->gmap->lightmap[cp->y * g->gmap->width + cp->x] || g->debug) {
                draw_charpoint(it->data, 
                        v->display_left + cp->x - left, 
                        v->display_top + cp->y - top);
            }
        }
        it = g_list_next(it);
    }
}

void draw_charpoint(CharPoint cp, int x, int y) {
    cc_putxy(cp->c, cp->color, x, y);
}

bool in_viewport(Viewport *v, int x, int y) {  // TODO union with Point struct
    int top = viewport_top(v);
    int left = viewport_left(v);
    int bottom = viewport_bottom(v);
    int right = viewport_right(v);
    return (x >= left) && (y >= top) && (x < right) && (y < bottom);
}

void draw_actor_self(Actor actor, Viewport *v) {
    int top = v->display_left - viewport_top(v);
    int left = v->display_top - viewport_left(v);
    draw_charpoint(actor_as_charpoint_cast(actor), left + actor->x, top + actor->y);
}

