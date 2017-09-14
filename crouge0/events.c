#include "events.h"
// matrix Role of actor X role of subject
// analogically make other actions, not just collision: (type, roleA, roleB)
EventsMap events;

void start_events() {
    events = events_init();
}

void end_events() {
    events_free(events);
}

void events_free(EventsMap events) {
    free(events);
}

EventsMap events_init() {
    int roles = RoleLength;
    int actions = ActionLength;
    int size = roles * roles * actions * sizeof(ActionFunc);
    EventsMap events = malloc(size);
    memset(events, 0, size);
    return events;
}

void event_register(Action action, Role object, Role subject, ActionFunc f) {
    int roles = RoleLength;
    size_t index = _3d_array_index(action, subject, object, roles);
    ActionFunc *fun = events + index;
    *fun = f;
}

int _3d_array_index(int x, int y, int z, int r) {
    return (z + x * r * r + y * r);
}

ActionFunc event_get(Action action, Role object, Role subject, G g) {
    UNUSED(g);
    int roles = RoleLength;
    size_t index = _3d_array_index(action, subject, object, roles);
    ActionFunc fun = *(events + index);
    return fun;
}

void _no_action(Action action, Actor actor, Actor subject, G g) {
    UNUSED(g);
    UNUSED(actor);
    UNUSED(subject);
    UNUSED(action);
    /* debuglog(g, "action undef"); */
}

