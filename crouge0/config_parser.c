#include "config_parser.h"
#include <string.h>

char delim = ':';

KVParam parse_dsv_kv_line(char *line) {
    char *value = strchr(line, delim);
    *value = '\0';
    KVParam kvp = { line, value + 1};
    return kvp;
}
