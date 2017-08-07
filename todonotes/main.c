#include <stdio.h>
#include <dirent.h>
#include "notesbook.h"
#include <sys/stat.h>
#include <errno.h>

//+0 Note is text, date, title
//+1 NotesBook is a book file containing notes
//+2 Notes list in book
// First run: check if dir exist, else create, or exit
// 3 Serialization to text formats
//      store all notebooks in dir
//      one file = one notebook
//
// 4 cli crud first minimum:
//   program show = show all notebooks(name:len)
//   program show name = show notes of notebook
//   program book somename = add notebook
//   program unbook somename = rm book file
//   program note bookname name <input> = add note
//   program rm bookname name = rm note 
//
// future:
// x curses ui (cui)
// x daemon

static const char *version = "0.1";

int main(void) {
    printf("NOTES V%s @ %s\n", version, notebooks_path);

    DIR *notes_dir = opendir(notebooks_path);
    if (notes_dir) {
        closedir(notes_dir);
    } else {
		int rv = mkdir(notebooks_path, 0770);
		if (rv) {
			perror("Create note books dir");
			exit(1);
		}
        printf("Created note books dir @ %s\n", notebooks_path);
    }

	notes_dir = opendir(notebooks_path);
	printf("Opened note books dir.\n");
}
