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
        char *end;
        bool escape = true;
        char *last_end = text;
        /* skip escaped delim */
        while (escape) { 
            end = strchr(last_end, delim);
            if (!end) break;
            escape = (*(end - 1) == '\\') && (*(end - 2) != '\\'); // for escape \ itself
            if (escape) last_end = end + 1;
            /* printf("delim: %c %p pred %c\n", *end, end, *(end-1)); */
        }
        if (!end) break;
        /* trim leading whitespaces */
        char *start = text + strspn(text, " \t");
        /* check for comment */
        if (*start != '#' && *start != '\n') { 
			*end = '\0';
            f(start, data);
			*end = delim;
        }
        text = end + 1;
    }
}

char *read_whole_file(char *filename) {
    ensure_file(filename);
    FILE *fin = fopen(filename, "r");
    fseek(fin, 0L, SEEK_END);
    long fsize = ftell(fin);
    /* fseek(fin, 0, SEEK_SET); */
    rewind(fin);
    char *content = malloc(fsize + 1);
    fread(content, fsize, 1, fin);
    content[fsize] = 0;
    fclose(fin);
    return content;
}


EError global_error = Error_OK;

char *error_msg() {
    char *error_messages[] = {
        "ok",
        "This item not equiptable",
        "Already equiped at this slot",
    };

    return error_messages[global_error];
}

void ensure_file(char *filename) {
	if (access(filename, F_OK) == -1) {
		fprintf(stderr, "File %s not accessible.\n", filename);
		exit(1);
	}
}

void afree(void *data) {
    if (data) free(data);
}

CharPoint charpoint_at(GList *lst, int x, int y) {
    GList *it = lst;
    while (it) {
        CharPoint elem = it->data;
        if ((elem->x == x) && (elem->y == y)) {
            return elem;
        }
        it = g_list_next(it);
    }
    return NULL;
}

