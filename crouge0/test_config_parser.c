#include "config_parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

void testfun(char *s, void *d) {
    printf("[%s]\n", s);
    (void)(d);
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

    parse_file("./maps/info");
}
