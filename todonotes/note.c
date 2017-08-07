#include "note.h"
#include <errno.h>
#include <stdio.h>

const char *date_format = "%d.%m.%Y %H:%M:%S";

Note note_new(string title, string text) {
    time_t now = time(NULL);
    Note note = malloc(sizeof(struct Note));
    note->date = now;
    note->text = text;
    note->title = title;
    return note;
}

void note_free(Note note) {
    free(note->text);
    free(note->title);
    free(note);
}

char *note_show(Note note) {
    struct tm *time;
    int len = 0;
    time = localtime(&note->date);
    if (time == NULL) {
        perror("localtime");
        exit(2);
    }
    
    char time_str[80];
    int rv = strftime(time_str, sizeof(time_str), date_format, time);
    if (rv == 0) {
        perror("strftime");
        exit(2);
    }
    char *buf = NULL;
    len += strlen(time_str) + 1;
    len += strlen(note->title) + 1;
    buf = malloc(len);
    sprintf(buf, "%s:%s", time_str, note->title);

    return buf;
}

char *note_dump(Note note) {
    char *header = note_show(note);
    int len = strlen(header) + strlen(note->text) + 5;
    char *buf = malloc(len);
    snprintf(buf, len, "%s\n%s", header, note->text);
    free(header);
    return buf;
}
