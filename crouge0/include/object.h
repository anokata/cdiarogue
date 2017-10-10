#pragma once
#include "util.h"
#include "string.h"
#include "config_parser.h"
#include <glib.h>

#define OBJECT_TYPE(X) \
    X(ObjectTypeExit) \
    X(ObjectTypeEnter) \
    X(ObjectTypeStash) \
    X(ObjectTypeDoor) \
    X(ObjectTypeTrap) \

DECLARE_ENUM(ObjectType, OBJECT_TYPE)

typedef struct Object {
/* First (four) fields is CharPoint struct */
    char c;
    int x;
    int y;
    Color color; /* end of CharPoint */
    char *param;
    Funcvp action;
    ObjectType type;
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
