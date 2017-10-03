#pragma once
#include "util.h"
#include "string.h"
#include "config_parser.h"
#include <glib.h>

typedef struct Object {
    int x;
    int y;
    char c;
    char *param;
    Funcvp action;
} *Object;

typedef GList *Objects;

Object object_new(int x, int y, char c);
void object_free(Object object);
void object_print(Object object);
char *object_serialize(Object obj);
Object object_deserialize(Strings str);
void objects_save(char *filename, Objects objs);
Objects objects_load(char *filename);
void objects_free(Objects *objs);
void object_add(Objects *objs, Object obj);
