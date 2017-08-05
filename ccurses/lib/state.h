#pragma once
#ifndef __STATE__
#define __STATE__
#include <stdint.h>
#include <stdlib.h>

typedef int (*StateFunction)(void*);
typedef StateFunction *StateTable; // like 2D array [state id X event id]
typedef struct State {
    StateTable table;
    uint32_t num_states;
    uint32_t num_events;
    int current_state;
} *State;


State ss_make_state(uint32_t states_count, uint32_t events_count);
void ss_free_state(State s);
void ss_sethander(State state, int state_id, int event, StateFunction fun);
void ss_setstate(State state, int new_state);
StateFunction ss_gethandler(State state, int event);
int ss_handle(State state, int event, void *data);


#endif
