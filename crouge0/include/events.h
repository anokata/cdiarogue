#pragma once
#include "actor.h"
#include "g.h"

typedef enum Action {
    ActionCollide, 
    // ... 
    ActionLength} Action;

typedef void (*ActionFunc)(Action action, Actor actor, Actor subject, G g);
typedef ActionFunc *EventsMap;

EventsMap events_init();
void start_events();
void _no_action(Action action, Actor actor, Actor subject, G g);
ActionFunc event_get(Action action, Actor actor, Actor subject, G g);
void end_events();
void events_free(EventsMap events);
void event_register(Action action, Role object, Role subject, ActionFunc f);
int _3d_array_index(int x, int y, int z, int r);
