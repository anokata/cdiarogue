#include "events.h"
// Role of actor X Role of subject TODO END
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
// event_register(actions, role_a role_b action_func)

ActionFunc event_get(Action action, Actor actor, Actor subject, G g) {
    UNUSED(actor);
    UNUSED(subject);
    UNUSED(g);
    UNUSED(action);
    // 3d get
    //return events[action][actor->role][subject->role];
    return _no_action;
}

void _no_action(Action action, Actor actor, Actor subject, G g) {
    UNUSED(actor);
    UNUSED(subject);
    UNUSED(action);
    debuglog(g, "action undef");
}

