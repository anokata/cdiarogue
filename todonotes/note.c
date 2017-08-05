#include "note.h"

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
