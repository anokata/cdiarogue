#include "object.h"

Object object_new(int x, int y, char c) {
    Object obj = malloc(sizeof(struct Object));
    bzero(obj, sizeof(struct Object));
    obj->x = x;
    obj->y = y;
    obj->c = c;
    return obj;
}


void object_free(Object obj) {
    afree(obj->param);
    free(obj);
}


void object_print(Object object) {
    printf("Obj %c: [%d:%d] %s\n", object->c, object->x, object->y, object->param);
}
