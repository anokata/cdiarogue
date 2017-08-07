#include "notesbook.h"
#include <stdio.h>

int main() {
    printf("test notebooks\n");
    NotesBook b;
    b = nbook_new(g_strdup("name"));
    nbook_free(b);
}

