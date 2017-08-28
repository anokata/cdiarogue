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
    event_register(ActionCollide, RolePlayer, RoleNPC, _no_action);
    memset(events, 0, size);
    return events;
}

void event_register(Action action, Role object, Role subject, ActionFunc f) {
    int roles = RoleLength;
    ActionFunc *fun = (events +
        (roles * roles * action) + (roles * subject + object));
    // TODO END
    /* *fun = f; */
}

ActionFunc event_get(Action action, Actor actor, Actor subject, G g) {
    UNUSED(actor);
    UNUSED(subject);
    UNUSED(g);
    UNUSED(action);
    int roles = RoleLength;
    // 3d get
    ActionFunc fun = *(events +
        (roles * roles * action) + (roles * subject->role + actor->role));
    return fun;
    /* return _no_action; */
}

void _no_action(Action action, Actor actor, Actor subject, G g) {
    UNUSED(actor);
    UNUSED(subject);
    UNUSED(action);
    debuglog(g, "action undef");
}

