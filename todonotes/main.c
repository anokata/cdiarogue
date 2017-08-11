#include <stdio.h>
#include <dirent.h>
#include "notesbook.h"
#include <sys/stat.h>
#include <errno.h>

/* #define DEBUG */
//+0 Note is text, date, title
//+1 NotesBook is a book file containing notes
//+2 Notes list in book
//+First run: check if dir exist, else create, or exit
// 3 Serialization to text formats
//      store all notebooks in dir
//			store note to string
//		format note:
//		title:01.29.2018 21:22:11
//		text .....
//		....
//		%%
//		
//      one file = one notebook
//+3.1 dump note
//+3.2 load note
//+3.3 dump and save notesbook
//+3.4 load notesbook
// 3.5 load all notesbooks from dir? list notesbooks?
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
// find, remove,
// x curses ui (cui)
// x daemon

#define dprint(expr) printf(#expr "= (%d)\n", expr);
#define swap(t, x, y) { t temp = x; x = y; y = temp; }

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
    int x = 2; int y = 80;
   swap(int, x, y);
   dprint(x - y);

	
	closedir(notes_dir);
}
