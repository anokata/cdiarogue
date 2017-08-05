#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
//optarg

#define BF_MEMORY_SIZE 30000
#define BF_LOOPS_SIZE 1000
#define BF_OUT_SIZE 1000
typedef struct Machine {
    uint8_t *data; 
    uint8_t dp; // Data pointer
    char *code; 
    uint8_t ip; // Instruction pointer 
    uint8_t ii; // Output index
    char *input;
    uint8_t input_size;
    uint8_t oi; // Output index
    char *output;
    uint32_t *loops;
    uint32_t code_size;
} Machine;

Machine* make_machine(char* code, char* input) {
    Machine* m = malloc(sizeof(Machine));
    m->data = malloc(BF_MEMORY_SIZE);
    m->loops = calloc(1, BF_LOOPS_SIZE);
    memset(m->loops, 0, BF_LOOPS_SIZE);
    m->dp = 0;
    m->ip = 0;
    m->oi = 0;
    m->ii = 0;
    m->code = code;
    m->code_size = strlen(code);
    m->output = malloc(BF_OUT_SIZE);
    m->input = input;
    m->input_size = strlen(input);

    uint32_t *loop_stack = malloc(BF_LOOPS_SIZE);
    int l = 0;
    for (int i = 0; i < strlen(code); i++) {
        if (code[i] == '[') {
            loop_stack[l++] = i;
        }
        if (code[i] == ']') {
            l--;
            m->loops[loop_stack[l]] = i;
            m->loops[i] = loop_stack[l];
        }
    }
    free(loop_stack);

    return m;
}

void down_machine(Machine* m) {
    free(m->output);
    free(m->loops);
    free(m->data);
    free(m);
}

int machine_reset(Machine* m) {
    m->dp = 0;
    m->ip = 0;
    m->oi = 0;
    m->ii = 0;
    memset(m->output, 0, BF_OUT_SIZE);
    memset(m->data, 0, BF_MEMORY_SIZE);
    return 1;
}

char* get_data_str(Machine* m) {
    char buf[128];
    static char str[1024];
    const int maxv = 64;
    str[0] = ' ';
    str[1] = 0;
    for (int base = 0; base < maxv; base += 16) {
        sprintf(buf, " 0x%04X:    ", base);
        strcat(str, buf);
        for (int i = 0; i < 16; i++) {
            sprintf(buf, "%02X ", m->data[base + i]);
            strcat(str, buf);
        }
        for (int i = 0; i < 16; i++) {
            sprintf(buf, "%c", m->data[base + i]);
            strcat(str, buf);
        }
        sprintf(buf, "\n ");
        strcat(str, buf);
    }
    return str;
}

char* get_machine_state_str(Machine* m) {
    char buf[128];
    static char str[1024];
    str[0] = ' ';
    str[1] = 0;
    sprintf(buf, "IP: %d  DP: %d \n", m->ip, m->dp);
    strcat(str, buf);
    sprintf(buf, " II: %d  OI: %d \n", m->ii, m->oi);
    strcat(str, buf);
    sprintf(buf, " OUT: '%s'\n", m->output);
    strcat(str, buf);
    return str;
}

int print_machine(Machine* m) {
    printf("%s\n", m->code);
    for (int i = 0; i < m->ip; i++) {
        printf(" ");
    }
    printf("^\n");
    printf("%s", get_machine_state_str(m));
    printf("Data:\n%s", get_data_str(m));
    printf("Loops: [%p]:  ", m->loops);
    for (int i = 0; i < BF_LOOPS_SIZE/sizeof(uint32_t); i++) {
        if (m->loops[i] && i < m->loops[i]) {
            printf("%d->%d  ", i, m->loops[i]);
        }
    }
    printf("\n");
    return 1;
}

void machine_cmd_dpinc(Machine* m) {
    m->dp++;
}

void machine_cmd_dpdec(Machine* m) {
    m->dp--;
}

void machine_cmd_datainc(Machine* m) {
    m->data[m->dp]++;
}

void machine_cmd_datadec(Machine* m) {
    m->data[m->dp]--;
}

void machine_cmd_out(Machine* m) {
    m->output[m->oi++] = m->data[m->dp];
}

void machine_cmd_in(Machine* m) {
    if (m->ii < m->input_size) {
        m->data[m->dp] = m->input[m->ii++];
    } else {
        m->data[m->dp] = 0;
    }
}

void machine_cmd_loop_start(Machine* m) {
    if (m->data[m->dp] == 0) {
        m->ip = m->loops[m->ip];
    }
}

void machine_cmd_loop_end(Machine* m) {
    if (m->data[m->dp] != 0) {
        m->ip = m->loops[m->ip];
    }
}

int machine_step(Machine* m) {
    switch (m->code[m->ip]) {
        case '>':
            machine_cmd_dpinc(m);
            break;
        case '<':
            machine_cmd_dpdec(m);
            break;
        case '+':
            machine_cmd_datainc(m);
            break;
        case '-':
            machine_cmd_datadec(m);
            break;
        case '.':
            machine_cmd_out(m);
            break;
        case ',':
            machine_cmd_in(m);
            break;
        case '[':
            machine_cmd_loop_start(m);
            break;
        case ']':
            machine_cmd_loop_end(m);
            break;
    }
    m->ip++;
    return 1;
}

void test_instructions(Machine* m) {
    machine_cmd_dpinc(m);
    machine_cmd_datainc(m);
    machine_cmd_dpinc(m);
    machine_cmd_datadec(m);
    machine_cmd_out(m);
    machine_cmd_dpdec(m);
    machine_cmd_out(m);
    machine_cmd_dpinc(m);
    machine_cmd_out(m);
    machine_cmd_in(m);
    machine_cmd_out(m);
    machine_cmd_dpinc(m);
    machine_cmd_in(m);
    machine_cmd_out(m);
}

int machine_run(Machine* m) {
    while (m->ip < m->code_size) {
        machine_step(m);
        //print_machine(m);
        //usleep(100000);
    }
    return 1;
}

int exit_func(Machine* m) {
    return 0;
}

int help_func(Machine* m) {
    static const char help[] = "Possible commands: \n"
        "run\texit\n"
        "print\t"
        "step\t"
        "reset\n"
        "c\t"
        "\n";
    printf("%s\n", help);
    return 1;
}

int enter_curses_mode(Machine*);

typedef int (*cmd_func)(Machine*);

typedef struct {
    char cmd[30];
    cmd_func func;
} Command;
Command commands[] = {
    {"run", machine_run},
    {"print", print_machine},
    {"p", print_machine},
    {"pr", print_machine},
    {"step", machine_step},
    {"s", machine_step},
    {"Reset", machine_reset},
    {"R", machine_reset},
    {"exit", exit_func},
    {"q", exit_func},
    {"quit", exit_func},
    {"help", help_func},
    {"h", help_func},
    {"c", enter_curses_mode},
};

cmd_func find_char_func(char c) {
    for (int i = 0; i < sizeof(commands)/sizeof(Command); i++) {
        if (commands[i].cmd[0] == c) {
            return commands[i].func;
        }
    }
    return NULL;
}

cmd_func find_cmd_func(char *cmd) {
    for (int i = 0; i < sizeof(commands)/sizeof(Command); i++) {
        if (strcmp(commands[i].cmd, cmd) == 0) {
            return commands[i].func;
        }
    }
    return NULL;
}

#define INPUT_BUF 200
int repl_one(Machine* m) {
    static const char promt[] = " > ";
    printf("%s", promt);

    char buf[INPUT_BUF];
    fgets(buf, INPUT_BUF, stdin);
    char cmd[INPUT_BUF];
    char arg1[INPUT_BUF];
    sscanf(buf, "%s %s", cmd, arg1);
    cmd_func f = find_cmd_func(cmd);
    if (f) {
        return f(m);
    } else {
        printf("Command '%s' not found! [h for help]\n", cmd);
        return 1;
    }
    return 1;
}

void repl(Machine* m) {
    while(repl_one(m));
}

void test_m() {
    char input[] = "some\xFF";
    char code[] = ",+[-.,+]";
    Machine* m = make_machine(code, input);
    printf("** Machine maked.\n");
    repl(m);
    //machine_run(m);
    //test_instructions(m);
    //print_machine(m);
    down_machine(m);
}

// curses
#define MAX_WINDOWS 4
static int width;
static int heigth;
static WINDOW* wins[MAX_WINDOWS];

void curses_init() {
	initscr();
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_YELLOW);
	init_pair(3, COLOR_GREEN, COLOR_WHITE);
	init_pair(4, COLOR_BLACK, COLOR_WHITE);
	init_pair(5, COLOR_WHITE, COLOR_BLACK);
	attron(COLOR_PAIR(4));
	refresh();
    width = getmaxx(stdscr);
    heigth = getmaxy(stdscr);
    curs_set(0);
}

void curses_end() {
	endwin();
	printf("* End curses\n");
}

WINDOW* make_window(int n, int m) {
	WINDOW* win = newwin(heigth/2, width/2, n * (heigth/2), m * (width/2));
	wattron(win, 0);
	wbkgdset(win, COLOR_PAIR(5)  | ' ');
	wclear(win);
    return win;
}

void win_refresh(Machine* m) {
    for (int i = 0; i < MAX_WINDOWS; i++) {
        WINDOW* win1 = wins[i];
        box(win1, 0, 0);
        refresh();
        wrefresh(win1);
    }
}

void win_help_refresh(Machine* m) {
    WINDOW* win = wins[3];
    if (m) {
        wmove(win, 1, 0);
        wprintw(win, "%s", " s - one step\n");
        wrefresh(win);
    }
    box(win, 0, 0);
    wmove(win, 0, 1);
	wprintw(win, "Help");
    wrefresh(win);
}

void win_state_refresh(Machine* m) {
    WINDOW* win = wins[2];
    if (m) {
        wmove(win, 1, 0);
        wprintw(win, "%s", get_machine_state_str(m));
        wrefresh(win);
    }
    box(win, 0, 0);
    wmove(win, 0, 1);
	wprintw(win, "State");
    wrefresh(win);
}

void win_data_refresh(Machine* m) {
    WINDOW* win1 = wins[1];
    if (m) {
        wmove(win1, 1, 0);
        wprintw(win1, "%s", get_data_str(m));
        wrefresh(win1);
    }
    box(win1, 0, 0);
    wmove(win1, 0, 1);
	wprintw(win1, "Data");
    wrefresh(win1);
}

void win1_refresh(Machine* m) {
    WINDOW* win1 = wins[0];
    wmove(win1, 0, 1);
	wprintw(win1, "Code");
    if (m) {
        wmove(win1, 1, 2);
        for (int i = 0; i < strlen(m->code); i++) {
            if (i != m->ip) {
                waddch(win1, m->code[i]);
            } else {
                waddch(win1, m->code[i] | COLOR_PAIR(1) | A_BOLD);
            }
        }
        wrefresh(win1);
    }
}

void makeWins() {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            WINDOW* win = (make_window(i, j));
            wins[i*2+j] = win;
        }
    }
    win_refresh(NULL);
}

void curses_allwin_refresh(Machine* m) {
        win_refresh(m);
        win1_refresh(m);
        win_data_refresh(m);
        win_state_refresh(m);
        win_help_refresh(m);
		refresh();
}

void processInput(Machine* m) {
	int ch = getch();
	while(ch != 'q') {
		clear();
		//if (ch != '\t');
        cmd_func f = find_char_func(ch);
        if (f) {
            f(m);
        }

        curses_allwin_refresh(m);
		//mvprintw(0, 0, "q = exit;");
		ch = getch();
	}
}

int enter_curses_mode(Machine* m) {
    curses_init();
    makeWins();
    curses_allwin_refresh(m);
    processInput(m);
    curses_end();
    return 0;
}

//TODO: interactive commands: view state, make step, run, reset, break at, 
// mem edit, input edit, code edit, loops view colors, Exec instruction
// forth lisp...
// highlight current instruction and data byte in colors
// code and data editor
int main(int argc, char** argv) {
    printf("* Brainfuck interpreter v 0.0 (q)\n");
    test_m();
    printf("* Quited.");
    return 0;
}
