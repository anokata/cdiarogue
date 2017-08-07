#include "notesbook.h"

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
