#include "config_parser.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

char delim = ':';
char tag_start = '[';

int _detect_columns(char *lines) {
	/* simple delim counter */
	int columns = 0;
	while (*lines != '\n') {
		if (*lines++ == delim) columns++;
	}
	return columns + 1;
}

int _detect_lines(char *lines) {
	int lines_count = 0;
	while (*lines) {
		if (*lines++ == '\n') lines_count++;
	}
	return lines_count + 1;
}

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
// 3. detect columns count
// 4. parse file
// TODO support of escaped \:

void _add_to_strings(char *value, void *data) {
	/* cast data to pointer to array to strings */
    Strings strings = *(char ***)data;
    Strings *pstrings = (char ***)data;
    /* duplicate with convert escaped chars */
    strings[0] = g_strcompress(value);
    (*pstrings)++;
}

Strings parse_dsv_line(char *str, int columns) {
    columns++;
    Strings strings = malloc(sizeof(str) * columns);
    bzero(strings, sizeof(str) * columns);
    Strings it = strings;
    for_every_part(str, delim, _add_to_strings, &it);
    return strings;
}

void free_dsv_strings(Strings s) {
    if (s == NULL) return;
    Strings orig = s;
    while (*s) {
        free(*s);
        *s = NULL;
        s++;
    }
    free(orig);
}

void _add_string_to_table(char *value, void *data) {
    DSVTable *table = data;
	table->table[0] = parse_dsv_line(value, table->columns);
	table->table++;
}

StringTable parse_dsv_table(char *lines) {
	int columns = _detect_columns(lines) + 1;
	int lines_count = _detect_lines(lines) + 1;
    DEBUG_PRINT("C:%d L:%d\n", columns, lines_count);

	StringTable table = malloc(sizeof(lines) * lines_count);
	bzero(table, sizeof(lines) * lines_count);
	DSVTable dsv_table;
	dsv_table.columns = columns - 1;
	dsv_table.lines = lines_count - 1;
	dsv_table.table = table;
	for_every_part(lines, '\n', _add_string_to_table, &dsv_table);
	return table;
}

void free_dsv_table(StringTable t) {
	if (!t) return;
	StringTable it = t;
	while (*it) {
		free_dsv_strings(*it);
		*it = NULL;
		it++;
	}
	free(t);
}

void dsv_table_print(StringTable table) {
	Strings ss;
	while (*table) {
        ss = *table++;
        printf("\n");
        while (*ss) {
            printf("* %s\n", *ss++);
        }
	}
}

StringTable parse_dsv_file(char *filename) {
	/* File must have at first line correct table header delimited columns - for count */
	StringTable table;
    char *content = read_whole_file(filename);
    table = parse_dsv_table(content);
    free(content);
	return table;
}

