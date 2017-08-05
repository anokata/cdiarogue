#include "state.h"
#include <stdio.h>

enum States {State_run, State_end, NUM_STATES};
enum Events {Event_draw, Event_key, E,  NUM_EVENTS};

int draw(void* data) {
    printf("run:draw\n");
    return 0;
}

int key(void* data) {
    printf("run:key\n");
    return 0;
}

int edraw(void* data) {
    printf("e:draw\n");
    return 0;
}

int ekey(void* data) {
    printf("e:key\n");
    return 0;
}

int main() {
    State state = ss_make_state(NUM_STATES, NUM_EVENTS);
    ss_sethander(state, State_run, Event_draw, draw);
    ss_sethander(state, State_run, Event_key, key);
    ss_sethander(state, State_run, E, edraw);
    ss_sethander(state, State_end, E, ekey);
    ss_setstate(state, State_run);


    ss_handle(state, Event_draw, NULL);
    ss_handle(state, Event_key, NULL);
    ss_handle(state, E, NULL);
    ss_setstate(state, State_end);
    ss_handle(state, E, NULL);

    ss_free_state(state);
}

