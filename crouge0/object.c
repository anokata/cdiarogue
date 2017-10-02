#include "object.h"
const char object_dump_format[] = "%d:%d:%c:%s:\n";

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


char *object_serialize(Object obj) {
    char *buf = malloc(BUFSIZE);
    snprintf(buf, BUFSIZE, object_dump_format,
            obj->x,
            obj->y,
            obj->c,
            (obj->param ? obj->param : " ")
    );
    return buf;
}

Object object_deserialize(Strings str) {
    Object obj = object_new(0, 0, 0);
    obj->x = atoi(str[0]);
    obj->y = atoi(str[1]);
    obj->c = str[2][0];
    obj->param = strdup(str[3]);
    return obj;
}
