#pragma once
#ifndef __UTIL__
#define __UTIL__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "lib/ccurses.h"

#define BUFSIZE 256
typedef char* string;

#define UNUSED(x) ((void)(x));

#ifdef DEBUG
#define DEBUG_PRINT(fmt, args...)    fprintf(stderr, fmt, ## args)
#else
#define DEBUG_PRINT(fmt, args...)
#endif

typedef struct Point {
    int x;
    int y;
} Point;

typedef struct CharPoint {
    char c;
    int x;
    int y;
    Color color;
} *CharPoint;

int fget_int_line(FILE *file);

typedef void (*EachStrFunc)(char* str, void *data);
void for_every_part(char *text, char delim, EachStrFunc f, void *data);

char *read_whole_file(char *filename);
void ensure_file(char *filename);

#define max(x, y) ((x) > (y) ? (x) : (y))
#define min(x, y) ((x) < (y) ? (x) : (y))


typedef enum EError {
    Error_OK,
    Error_Actor_NotEquptable,
    Error_Actor_SlotBusy
} EError;

char *error_msg();

struct IntPair {
    int a;
    int b;
};


#define DECLARE_ENUM(NAME, ELEMS) typedef enum NAME {\
    ELEMS(MAKE_ENUM)\
} NAME;\
NAME NAME##_from_str(char *str);

#define IMPLEMENT_ENUM(NAME, ELEMS) \
char *NAME##Names[] = {\
    ELEMS(MAKE_STRING)\
    NULL\
};\
NAME NAME##_from_str(char *str) {\
    ENUM_FROMSTR(NAME)\
}\


#define MAKE_STRING(STR) #STR,
#define MAKE_ENUM(X) X,

#define ENUM_FROMSTR(ENUM) \
    char **it = ENUM##Names;\
    ENUM val = 0;\
    while (*it) {\
        if (!strcmp(*it, str)) return val;\
        val++;\
        it++;\
    }\
    return 0;\

#endif

typedef void (*Funcvp)(void *);

void afree(void *data);
CharPoint charpoint_at(GList *lst, int x, int y);
