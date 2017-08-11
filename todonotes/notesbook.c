#include "notesbook.h"
#include <stdlib.h>
#include "util.h"

char separator[] = "\n%%\n";
static char separator_file[] = "%%\n"; //TODO extract

NotesBook nbook_new(char *name) {
    NotesBook book = malloc(sizeof(struct NotesBook));
    book->name = name;
    book->notes = NULL;
    return book;
}

void nbook_free(NotesBook book) {
    free(book->name);
    GList *note_node = book->notes;
    while (note_node != NULL) {
        note_free(note_node->data);
        note_node = g_list_next(note_node);
    }
    g_list_free(book->notes);
    free(book);
}

void nbook_add_note(NotesBook book, Note note) {
    book->notes = g_list_append(book->notes, note);
}

void nbook_print(NotesBook book) {
    printf("Notes book '%s'()\n",book->name);
    GList *note_node = book->notes;
    int i = 1;
    while (note_node != NULL) {
        Note note = note_node->data;
        printf("#%d. %s:%s\n", i++, note->title, note->text);
        note_node = g_list_next(note_node);
    }
}

char *nbook_dump(NotesBook book) {
    GList *note_node = book->notes;
	int len = 5;
	char *buf = malloc(len);
	bzero(buf, len);
    while (note_node != NULL) {
        Note note = note_node->data;
		char *note_str = note_dump(note);
		len += strlen(note_str) + 5;
		buf = realloc(buf, len);
		strcat(buf, note_str);
		strcat(buf, separator);
		free(note_str);
        note_node = g_list_next(note_node);
    }
	return buf;
}

void nbook_save(NotesBook book) {
    int len = strlen(book->name) + strlen(notebooks_path) + 2;
    char *filename = malloc(len);
    snprintf(filename, len, "%s%s", notebooks_path, book->name);
    FILE *fout = fopen(filename, "w+");
    if (!fout) {
        perror("fopen file for save");
        printf("%s\n", filename);
        exit(3);
    }
    char *dump = nbook_dump(book);
    fwrite(dump, strlen(dump), 1, fout);
    free(dump);
    free(filename);
    fclose(fout);
}

FILE *_open_nbook_file(const char *filename) {
    int len = strlen(filename) + strlen(notebooks_path) + 2;
    char *full_path = malloc(len);
    snprintf(full_path, len, "%s%s", notebooks_path, filename);
    DEBUG_PRINT("Full path: %s\n", full_path);

    FILE *fin;
    fin = fopen(full_path, "r");
    free(full_path);
    return fin;
}

Note note_read(char **content) {
    Note note = NULL;
    char *dump = NULL;
    char *note_str = NULL;
    note_str = strstr(*content, separator_file);
    if (note_str) *note_str = '\0';
    dump = malloc(strlen(*content) + 1);
    strcpy(dump, *content);
    if (note_str) *note_str = '\n';
    *content = note_str ? (note_str + strlen(separator_file)) : NULL;
    
    note = note_load(dump);
    free(dump);
    return note;
}

NotesBook nbook_load(const char *filename) {
    NotesBook book;
    book = nbook_new(g_strdup(filename));

    // read book file
    // Read until separator, send to read note from string, add note, until EOF
    FILE *fin = _open_nbook_file(filename);
    // read whole file, then strstr dup pass by note
    fseek(fin, 0, SEEK_END);
    long fsize = ftell(fin);
    fseek(fin, 0, SEEK_SET);
    char *content = malloc(fsize + 1);
    char *old_content = content;
    fread(content, fsize, 1, fin);
    content[fsize] = 0;
    fclose(fin);

    while (*content) {
        Note note = note_read(&content);
        nbook_add_note(book, note);
    }
    free(old_content);

    return book;
}
