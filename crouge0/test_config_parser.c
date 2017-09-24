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

void test_dsv_line(char *line, int col) {
    char *test1 = NULL;
    printf("DSV lines TEST\n");
    test1 = strdup(line);
    Strings ss = parse_dsv_line(test1, col);
    Strings it = ss;
    while (*it) {
        printf("- %s\n", *it++);
    }
    free_dsv_strings(ss);
    free(test1);
}

int main() {
    char *test1;
    test1 = strdup("abcd:123");
    KVParam p = parse_dsv_kv_line(test1);
    printf("%s : %s\n", p.key, p.value);
    assert(strcmp(p.key, "abcd") == 0);
    assert(strcmp(p.value, "123") == 0);
    free(test1);

    test1 = strdup("abcd:123\\:xxx:1231");
    for_every_part(test1, ':', testfun, NULL);
    free(test1);

    GHashTable *t = parse_file("./maps/map_0_0");
    g_hash_table_destroy(t);

    t = parse_file("./maps/map_0_0");
    g_hash_table_foreach(t, test_printKV, NULL);
    g_hash_table_destroy(t);

    test1 = NULL;
    printf("DSV one line TEST\n");
    test1 = strdup("ab:cded:fe:");
    Strings ss = parse_dsv_line(test1, 3);
    Strings it = ss;
    while (*it) {
        printf("- %s\n", *it++);
    }
    assert(strcmp(ss[0], "ab") == 0);
    assert(strcmp(ss[1], "cded") == 0);
    assert(strcmp(ss[2], "fe") == 0);
    assert(ss[3] == NULL);
    free_dsv_strings(ss);
    free(test1);

    test_dsv_line("a:_______\\:_13:zxcvzvvxzvvvvxxzvzxvxcvxzv*#!##!#z:00:", 4);

    printf("\nDSV lines TEST\n");
	test1 = strdup("ab:cd:\n12:34:\n");
	StringTable st = parse_dsv_table(test1);
    dsv_table_print(st);
	free_dsv_table(st);
	free(test1);

    printf("\nDSV file TEST\n");
    st = parse_dsv_file("./maps/map_0_0.tiles");
    dsv_table_print(st);
    free_dsv_table(st);

    st = parse_dsv_file("./maps/map_1_1.actors");
    dsv_table_print(st);
    free_dsv_table(st);

    /* escape tst */
    test1 = strdup("\\:x:1:2\\::3");
    for_every_part(test1, ':', testfun, NULL);
    free(test1);

}
