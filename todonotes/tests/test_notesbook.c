#include "notesbook.h"
#include <stdio.h>

int main() {
    printf("test notebooks %s\n", notebooks_path);
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
	printf("DUMP\n%s\n", dump);
	free(dump);

    nbook_save(b);
    nbook_free(b);


    b = nbook_load("name");
    nbook_print(b);

    nbook_free(b);

    nbook_list();

    nbook_file_new("test_book");
    nbook_file_new("test_book");
    nbook_list();
    nbook_file_rm("test_book");
    nbook_file_rm("test_book");
    nbook_list();
}

