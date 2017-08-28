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

// roles * roles * actions
EventsMap events_init() {
    int roles = RoleLength;
    int actions = ActionLength;
    int size = roles * roles * actions * sizeof(ActionFunc);
    EventsMap events = malloc(size);
    // for_each_event(events, _event_set_default_no_action);
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

ActionFunc event_get(Action action, Actor actor, Actor coactor, G g) {
    UNUSED(g);
    UNUSED(action);
    int roles = RoleLength;
    int object = actor->role;
    int subject = coactor->role;
    size_t index = _3d_array_index(action, subject, object, roles);
    ActionFunc fun = *(events + index);
    return fun;
    /* return _no_action; */
}

void _no_action(Action action, Actor actor, Actor subject, G g) {
    UNUSED(actor);
    UNUSED(subject);
    UNUSED(action);
    debuglog(g, "action undef");
}

