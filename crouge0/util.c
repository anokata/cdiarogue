#include "util.h"

int fget_int_line(FILE *file) {
    int value = 0;
    char * line = NULL;
    size_t len = 0;
    getline(&line, &len, file);
    sscanf(line, "%d", &value);
    if (line)
        free(line);
    return value;
}

void for_every_part(char *text, char delim, EachStrFunc f, void *data) { 
    while (*text) {
        char *end = strchr(text, delim);
        if (!end) break;
        *end = '\0';
        f(text, data);
        *end = delim;
        text = end + 1;
    }
}

char *read_whole_file(char *filename) {
    FILE *fin = fopen(filename, "r");
    fseek(fin, 0, SEEK_END);
    long fsize = ftell(fin);
    fseek(fin, 0, SEEK_SET);
    char *content = malloc(fsize + 1);
    fread(content, fsize, 1, fin);
    content[fsize] = 0;
    fclose(fin);
    return content;
}

