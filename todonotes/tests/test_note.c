#include <stdio.h>
#include "note.h"
#include <glib.h>

void note_test() {
    string t = strdup("hi");
    string x = strdup("text");
    Note n = note_new(t, x);
	char *free_me = note_show(n);
	printf("NOTE:%s\n", free_me);
	free(free_me);

	free_me = note_dump(n);
	printf("dump:%s\n", free_me);
	free(free_me);

    note_free(n);


    char *dump = g_strdup("title\n29.01.2018 21:22:11\ntext\n.....\n ....\n %%\n");
    n = note_load(dump);
	free_me = note_show(n);
	printf("NOTE loaded:%s\n", free_me);
	free(free_me);
    free(dump);

    note_free(n);
}

int main(void) {
    note_test();
}

