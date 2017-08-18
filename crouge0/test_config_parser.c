#include "config_parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

void testfun(char *s, void *d) {
    printf("[%s]\n", s);
    (void)(d);
}

void test_printKV(gpointer key, gpointer value, gpointer data) {
    char *skey = key;
    char *svalue = value;
    UNUSED(data);
    printf("%s : %s \n", skey, svalue);
}

int main() {
    char *test1;
    test1 = strdup("abcd:123");
    KVParam p = parse_dsv_kv_line(test1);
    printf("%s : %s\n", p.key, p.value);
    assert(strcmp(p.key, "abcd") == 0);
    assert(strcmp(p.value, "123") == 0);
    free(test1);

    test1 = strdup("abcd:123:xxx:1231");
    for_every_part(test1, ':', testfun, NULL);
    free(test1);

    GHashTable *t = parse_file("./maps/info");
    g_hash_table_destroy(t);

    t = parse_file("./maps/map_0_0");
    g_hash_table_foreach(t, test_printKV, NULL);
    g_hash_table_destroy(t);
}
