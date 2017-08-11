#include "command.h"
#include "notesbook.h"
#include <unistd.h>
#include <errno.h>
#define UNUSED(x) ((void)(x))

void print_help(char **params) {
    UNUSED(params);
    printf("Params: %s\n", params[0]);
    printf("help is\n");
}

void print_list(char **params) {
    UNUSED(params);
    UNUSED(notebooks_path);

    nbook_list();
}

char *_get_bookname(char **params) {
    char *bookname = params[0];
    if (bookname == NULL) {
        printf("no parametr\n");
        exit(6);
    }
    return bookname;
}

void print_nbook(char **params) {
    NotesBook book = nbook_load(_get_bookname(params));
    nbook_print(book);
    nbook_free(book);
}

void cmd_book(char **params) {
    char *bookname = _get_bookname(params);
    // Check if file exist then warning else create
    char *bookpath = nbook_path(bookname);
    if(access(bookpath, F_OK) != -1) {
        printf("Book %s already exists @ %s\n", bookname, bookpath);
        exit(6);
    } else {
        printf("Creating book %s\n", bookname);
        FILE *f = fopen(bookpath, "w+");
        if (!f) {
            fprintf(stderr, 
                    "Error creating book @ %s: %s\n", 
                    bookpath, 
                    strerror(errno));
            exit(6);
        }

        fclose(f);
    }   
}

void cmd_unbook(char **params) {
    char *bookname = _get_bookname(params);
    // Check if file exist then remove it else warning

}

Command Commands[] = {
    {"help", print_help},
    {"list", print_list},
    {"show", print_nbook},
    {"book", cmd_book},
    {"unbook", cmd_unbook},
};

static int commands_count = sizeof(Commands)/sizeof(Command);

Command cmd_get(char *name) {
    for (int i = 0; i < commands_count; i++) {
        if (strcmp(Commands[i].name, name) == 0) {
            return Commands[i];
        }
    }
    return Commands[0];
}


