#pragma once
#ifndef __CCURSES__
#define __CCURSES__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>

#include "color.h"
#include "state.h"
#include "util.h"

int width;
int heigth;

void cc_print(char *str, struct Color color);
void cc_printi(uint32_t value, struct Color color);
void cc_printxy(char *str, struct Color color, int x, int y);
void cc_put(char ch, struct Color color);
void cc_putxy(char ch, struct Color color, int x, int y);
void curses_init();
void processInput();
void curses_end();
void start();
void cc_refresh();

#endif
