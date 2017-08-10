#include "note.h"
#include <errno.h>
#include <stdio.h>

const char *date_format = "%d.%m.%Y %H:%M:%S";
char separator_file[] = "%%\n";

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
    sprintf(buf, "%s\n%s", note->title, time_str);

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

char *_parse_title(char **dump) { // one line
    char *str_title = NULL;
    char *line = strchr(*dump, '\n');
    if (line) *line = '\0';  // temporarily terminate the current line
    str_title = malloc(strlen(*dump) + 1);
    strcpy(str_title, *dump);
    if (line) *line = '\n';  // then restore newline-char, just to be tidy    
    *dump = line ? (line+1) : NULL;
    printf("title: %s\n", str_title);
    return str_title;
}

time_t _parse_date_str(char *date) {
    struct tm *time_full = malloc(sizeof(struct tm));
    memset(time_full, 0, sizeof(struct tm));
    if (strptime(date, date_format, time_full) == NULL) {
        perror("date format read");
        exit(4);
    }
    time_t time = mktime(time_full);
    free(time_full);
    return time;
}

time_t _parse_date(char **dump) {
    char *line = strchr(*dump, '\n');
    char *str_date = NULL;
    if (line) *line = '\0';
    str_date = malloc(strlen(*dump) + 2);
    strcpy(str_date, *dump);
    if (line) *line = '\n';
    *dump = line ? (line+1) : NULL;
    printf("date: [%s]\n", str_date);

    time_t date = _parse_date_str(str_date);
    free(str_date);
    return date;
}

char *_parse_content(char **dump) {
    char *str_content = NULL;
    char *line = strstr(*dump, separator_file);
    if (line) *line = '\0';
    str_content = malloc(strlen(*dump) + 2);
    strcpy(str_content, *dump);
    if (line) *line = '\n';
    *dump = line ? (line+1) : NULL;
    printf("str_content: [%s]\n", str_content);
    return str_content;
}

Note note_load(char *dump) {
    Note note = malloc(sizeof(struct Note));
    memset(note, 0, sizeof(struct Note));
    
    note->title = _parse_title(&dump);
    note->date = _parse_date(&dump);
    note->text = _parse_content(&dump);

    return note;
}

void _print_note(Note note) {
	char *free_me = note_dump(note);
	printf("%s\n", free_me);
	free(free_me);
}
