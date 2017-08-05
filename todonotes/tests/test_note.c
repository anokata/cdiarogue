#include <stdio.h>
#include "note.h"

void note_test() {
    string t = strdup("hi");
    string x = strdup("text");
    Note n = note_new(t, x);

    note_free(n);
}

int main(void) {
    note_test();
}

