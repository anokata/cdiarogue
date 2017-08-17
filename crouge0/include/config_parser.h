#pragma once

typedef struct KVParam {
    char *key;
    char *value;
} KVParam;

KVParam parse_dsv_kv_line(char *line);
