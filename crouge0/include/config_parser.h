#pragma once
#include <glib.h>
#include "util.h"

typedef struct KVParam {
    char *key;
    char *value;
} KVParam;

KVParam parse_dsv_kv_line(char *line);

GHashTable *parse_file(char *filename);

typedef char **Strings;
Strings parse_dsv_line(char *str, int columns);
void free_dsv_strings(Strings s);
