#pragma once
#include <glib.h>
#include "util.h"

typedef struct KVParam {
    char *key;
    char *value;
} KVParam;

KVParam parse_dsv_kv_line(char *line);

GHashTable *parse_file(char *filename);

/* array of pointers of strings */
typedef char **Strings;
/* array of pointers to arrays of strings*/
typedef Strings *StringTable;

typedef struct DSVTable {
	StringTable table;
	int columns;
} DSVTable;

Strings parse_dsv_line(char *str, int columns);
void free_dsv_strings(Strings s);
StringTable parse_dsv_table(char *lines, int columns);
void free_dsv_table(StringTable t);
