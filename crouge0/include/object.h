#pragma once
#include "util.h"
#include "string.h"
#include <glib.h>

typedef struct Object {
    int x;
    int y;
    char c;
    char *param;
} *Object;

Object object_new(int x, int y, char c);
void object_free(Object object);
void object_print(Object object);
