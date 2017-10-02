#include "object.h"
#include "config_parser.h"
#include <stdio.h>
#include <assert.h>

void test_create() {
    Object o = object_new(1, 2, 'z');
    object_print(o);
    object_free(o);
}

void test_serialization() {
    Object o = object_new(1, 2, 'z');
    char *x = object_serialize(o);
    printf("dump:%s\n", x);
    free(x);
    object_free(o);

    char *test = strdup("1:2:$:hello:\n");
    Strings s = parse_dsv_line(test, 9);
    Object z = object_deserialize(s);
    object_print(z);
    object_free(z);
    free(test);
    free_dsv_strings(s);

}

int main() {
    test_create();
    test_serialization();
}
