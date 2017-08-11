#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include "notesbook.h"
#include "command.h"

/* #define DEBUG */
/* Rule of dev: First make full basic functionality then detailed expand it. */

/* Note consist of text, date, and title */
//+1 NotesBook is a book file containing notes
//+2 Notes list in book
//+First run: check if dir exist, else create, or exit
//+3 Serialization to text formats
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
//+3.5 load all notesbooks from dir? list notesbooks?
//
//.4 cli crud first minimum:
// . parsing args
// + program list = show all notebooks(name:len)
// + program show name = show notes of notebook
// + program book somename = add notebook
// + program unbook somename = rm book file
//   program note bookname name <input> = add note (load, add, save or create and dump append to book file)
//   program rm bookname name = rm note (load, del in list, serialize)
//
// future:
// show notes short/long modes
// find, remove, edit note text
// x curses ui (cui)
// x daemon
// x web ui
// x http api

#define dprint(expr) printf(#expr "= (%d)\n", expr);
#define swap(t, x, y) { t temp = x; x = y; y = temp; }

static const char *version = "0.2.2";
// RULES:
// Global names namig!!! how? or drop it
// comments

void ensure_notes_dir_exist() {
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
}

int main(int argc, char *argv[]) {
    printf("Notes books v%s\t2017 path@ %s\n", version, notebooks_path);
    ensure_notes_dir_exist();

    /* printf("C:%d\n", argc); */
    if (argc == 1) {
        printf("using: tn [CMD] [PARAM]\n"
               "Avaliable CMD is:\n"  // TO help
               "\tlist - List notes books in default path\n"
               "\tshow\t BOOKNAME - show book of notes\n"
               "\tbook\t BOOKNAME - create book\n"
               "\tunbook\t BOOKNAME - delete book\n"
                );
        return 0;
    }
    char *command = argv[1];
    char **params = &argv[2];
    printf("Cmd: %s\n", command);
    cmd_get(command).run(params);


	/* notes_dir = opendir(notebooks_path); */
	/* printf("Opened note books dir.\n"); */
    /* int x = 2; int y = 80; */
   /* swap(int, x, y); */
   /* dprint(x - y); */
	/* closedir(notes_dir); */
}
