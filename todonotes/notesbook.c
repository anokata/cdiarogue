#include "notesbook.h"
#include <stdlib.h>

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
	char separator[] = "\n%%\n";
	int len = 5;
	char *buf = malloc(len);
	bzero(buf, len);
    while (note_node != NULL) {
        Note note = note_node->data;
		char *note_str = note_dump(note);
		len += strlen(note_str) + 5;
		buf = realloc(buf, len);
		/* snprintf(buf, len, "%s\n%%%%\n", note_str); */
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
