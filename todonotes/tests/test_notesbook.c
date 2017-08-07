#include "notesbook.h"
#include <stdio.h>

int main() {
    printf("test notebooks\n");
    NotesBook b;
    b = nbook_new(g_strdup("name"));

    Note n = note_new(g_strdup("note1"), g_strdup("content"));
    nbook_add_note(b, n);
    n = note_new(g_strdup("note2"), g_strdup("<content>\nis loooong\n."));
    nbook_add_note(b, n);
    n = note_new(g_strdup("note3"), g_strdup("<TEX>"));
    nbook_add_note(b, n);

    nbook_print(b);

	char *dump = nbook_dump(b);
	printf("DUMP: %s\n", dump);
	free(dump);

    nbook_free(b);
}

