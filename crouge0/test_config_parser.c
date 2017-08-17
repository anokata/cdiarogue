#include "config_parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
    char *test1;
    test1 = strdup("abcd:123");
    KVParam p = parse_dsv_kv_line(test1);
    printf("%s : %s", p.key, p.value);
    free(test1);
}
