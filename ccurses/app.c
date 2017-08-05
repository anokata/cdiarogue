#include "app.h"

enum States {State_run, State_end,  NUM_STATES};
enum Events {Event_draw, Event_key,  NUM_EVENTS};
/* GLOBAL */
State state;

// draw func 
// input func
// menu, state

void processInput() {
	int ch = getch();
	while(ch != 'q') {
        ss_handle(state, Event_draw, 0);
        ss_handle(state, Event_key, (void*)(uint64_t)ch);
		ch = getch();
	}
}

int key_run(void* data) {
    char key = (uint64_t)data;
    cc_printi(key, cd_green);
    return 0;
}

int draw(void* data) {
    clear();
    cc_putxy('D', cb_yellow, 3, 2);
    return 0;
}


void state_init() {
    state = ss_make_state(NUM_STATES, NUM_EVENTS);
    ss_sethander(state, State_run, Event_draw, draw);
    ss_sethander(state, State_run, Event_key, key_run);
    ss_setstate(state, State_run);
}

void start() {
    curses_init();
		cc_putxy('X', cd_yellow, 1, 2);
		cc_print("\nW:H = ", cd_red);
        cc_printi(width, cd_white);
		cc_print(" : ", cd_red);
        cc_printi(heigth, cd_white);
    state_init();

    processInput();
    ss_free_state(state);
    curses_end();
}
