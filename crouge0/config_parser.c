#include "config_parser.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

char delim = ':';
char tag_start = '[';

KVParam parse_dsv_kv_line(char *line) {
    char *value = strchr(line, delim);
    *value = '\0';
    KVParam kvp = { line, value + 1};
    DEBUG_PRINT("Parse: %s : %s\n", line, value + 1);
    return kvp;
}

void print_ss_kvparam(KVParam param) {
    printf("%s : %s\n", param.key, param.value);
}

bool is_tag(char *str) {
    return tag_start == str[0];
}

void _add_kv_to_hash(char *line, void *hash_table) {
    GHashTable *table = hash_table;
    if (is_tag(line)) {
    
    } else {
        KVParam param = parse_dsv_kv_line(line);
        /* dup every param key val */
        g_hash_table_insert(table, g_strdup(param.key), g_strdup(param.value));
        /* print_ss_kvparam(param); */
    }
}

GHashTable *parse_file(char *filename) {
    /* for auto free key and val when destroy */
    GHashTable *table = g_hash_table_new_full(g_str_hash, g_str_equal, free, free);
    char *content = read_whole_file(filename);
    for_every_part(content, '\n', _add_kv_to_hash, table);
    free(content);
    return table;
}
// get by key : int len = get_kv(kvp, "len");

// parse dsv to strings list
// file:
// value:some:other:123
// value2:s0me:0t4er:89
// to array of pointers ot array of pointers to strings
// char *parsed_file = [x, y] -> x = ["value", "some" ...
// 1. parse line to strings array
// 2. parse file to array of parsed lines
// TODO support of escaped \:
