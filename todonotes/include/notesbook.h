#pragma once

#include <glib.h>
#include "note.h"

typedef struct NotesBook {
    GList *notes;
    char *name;
} *NotesBook;

NotesBook nbook_new(char *name);
void nbook_free(NotesBook book);
