#include "config_parser.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Valgrind it TODO
char delim = ':';

KVParam parse_dsv_kv_line(char *line) {
    char *value = strchr(line, delim);
    *value = '\0';
    KVParam kvp = { line, value + 1};
    return kvp;
}

void print_ss_kvparam(KVParam param) {
    printf("%s : %s\n", param.key, param.value);
}

void _add_kv_to_hash(char *line, void *hash_table) {
    GHashTable *table = hash_table;
    KVParam param = parse_dsv_kv_line(line);
    g_hash_table_insert(table, param.key, param.value);
    //print_ss_kvparam(param);
}

GHashTable *parse_file(char *filename) {
    GHashTable *table = g_hash_table_new(g_str_hash, g_str_equal);
    char *content = read_whole_file(filename);
    for_every_part(content, '\n', _add_kv_to_hash, table);
    free(content);
    return table;
}
// get by key : int len = get_kv(kvp, "len");
