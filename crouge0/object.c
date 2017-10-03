#include "object.h"
const char object_dump_format[] = "%d:%d:%c:%s:\n";
const char obj_file_header[] = "x:y:char:param:\n";
const char obj_file_type[] = "# vi: filetype=sh\n";

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

void object_add(Objects *objs, Object obj) {
    *objs = g_list_append(*objs, obj);
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

void objects_save(char *filename, Objects objs) {
    GList *it = objs;
    FILE *out = fopen(filename, "w+");
    fwrite(obj_file_header, strlen(obj_file_header), 1, out);
    fwrite(obj_file_type, strlen(obj_file_type), 1, out);

    while (it) {
        Object obj = it->data;
        char *line = object_serialize(obj);
        fwrite(line, strlen(line), 1, out);
        free(line);
        it = g_list_next(it);
    }

    fclose(out);
}

Objects objects_load(char *filename) {
    Objects objs = NULL; 
    filename = strdup(filename);
    StringTable st = parse_dsv_file(filename);
    StringTable it = &st[1]; // first line is header
    do {
        Object obj = object_deserialize(*it);
        object_add(&objs, obj); 
        DEBUG_PRINT("load obj: %s\n", obj->param);
    } while (*++it);

    free_dsv_table(st);
    free(filename);
    return objs;
}

void objects_free(Objects *objs) {
    if (!*objs) return;
    GList *it = *objs;
    while (it) {
        object_free((Object) it->data);
        it = g_list_next(it);
    }
    g_list_free(*objs);
    *objs = NULL;
}
